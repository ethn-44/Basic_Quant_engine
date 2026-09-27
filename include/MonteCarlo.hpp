#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP
#include <random>
#include <cstddef>
#include "BlackScholes.hpp"
#include "Payoff.hpp"
#include "PathPayoff.hpp"
#include "reg.hpp"
#include <omp.h>
#include <cmath>
class MonteCarloEngine{
    private:
    int initial_seed_;
    std::mt19937_64 gen_;
    public:
    explicit MonteCarloEngine(int seed=42): initial_seed_(seed), gen_(seed) {}
    void reset_seed() {
        gen_.seed(initial_seed_);
    }
    template <typename ModelType, typename PayoffType>
    double price_bs(const ModelType& model, const PayoffType& payoff, double maturity, std::size_t num_sims){ //num_sims est pair
        std::normal_distribution<double> dist(0.0,1.0);
        double c=0.0;
        const double r = model.rate();
        const double sigma = model.vol();
        const double drift = (r - 0.5 * sigma * sigma) * maturity;
        const double diffusion = sigma * std::sqrt(maturity);
        const double discount_factor = std::exp(-r * maturity);
        #pragma omp parallel 
        {
        int thread_id=omp_get_thread_num();
        std::mt19937_64 local_gen(gen_()+ thread_id);
        #pragma omp for reduction(+:c)
        for (std::size_t i = 0; i < num_sims/2; ++i){
            double normal=dist(local_gen);
            double S_t_1=model.spot_at_maturity(drift,diffusion,normal);
            double S_t_2=model.spot_at_maturity(drift,diffusion,-normal);
            c+=payoff(S_t_1)+payoff(S_t_2);
        }
        }
        return discount_factor * (c/static_cast<double>(num_sims));
    }
    
    template <typename ModelType, typename PayoffType>
        double price_path_dependant(const ModelType& model, const PayoffType& payoff, double maturity, std::size_t num_sims, std::size_t num_steps) {
        const double r = model.rate();
        double c = 0.0;
        const double factor_discount = std::exp(-r * maturity);

        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            std::mt19937_64 local_gen(gen_() + thread_id);
            #pragma omp for reduction(+:c)
            for (std::size_t i = 0; i < num_sims; ++i) {
                auto path = model.generate_path(maturity, num_steps, local_gen);
                if constexpr (std::is_invocable_v<PayoffType, const std::vector<double>&>) {
                    c += payoff(path);
                } 
                else {
                    c += payoff(path.back());
                }
            }
        }
        return (c / static_cast<double>(num_sims)) * factor_discount;
    }
    template <typename ModelType, typename PayoffType>
    double price_american(const ModelType& model,const PayoffType& payoff,double maturity,std::size_t num_sims,std::size_t num_steps){
        const double r = model.rate();
        const double factor_discount=std::exp(-r*(maturity/static_cast<double>(num_steps)));
        std::vector<std::vector<double>> mat(num_sims, std::vector<double>(num_steps + 1));
        #pragma omp parallel
        {
        int thread_id=omp_get_thread_num();
        std::mt19937_64 local_gen(gen_()+ thread_id);
        #pragma omp for
        for (std::size_t i=0;i<num_sims;++i){
            mat[i]=model.generate_path(maturity,num_steps,local_gen);
            }
        }
        std::vector<double> cash_flows(num_sims);
        for (std::size_t m = 0; m < num_sims; ++m) {
            cash_flows[m] = payoff(mat[m][num_steps]);
        }
        std::vector<double> x_itm;
        std::vector<double> y_itm;
        std::vector<std::size_t> itm_index;
        std::vector<double> acalc; 
        x_itm.reserve(num_sims);
        y_itm.reserve(num_sims);
        itm_index.reserve(num_sims);
        acalc.reserve(3);
        for (int step = static_cast<int>(num_steps) - 1; step >= 1; --step) {
            x_itm.clear();
            y_itm.clear();
            itm_index.clear();
            acalc.clear();
            for (std::size_t j = 0; j < num_sims; ++j){
                cash_flows[j]=factor_discount*cash_flows[j];
            }
            for (std::size_t u = 0; u < num_sims; ++u){
                double spot=mat[u][step];
                double payoff_imm=payoff(spot);
                if(payoff_imm>0){
                    x_itm.push_back(spot);
                    y_itm.push_back(cash_flows[u]);
                    itm_index.push_back(u);
                }
            }
            if (x_itm.size()>=3){
                acalc=reg(x_itm,y_itm,model.spot());
                #pragma omp parallel for schedule(static)
                for (std::size_t k=0; k<itm_index.size();k++){
                std::size_t m=itm_index[k];
                double spot = x_itm[k];
                double immediate_payoff = payoff(spot);
                double continuation_val = predict(acalc,spot,model.spot());

                if (immediate_payoff > continuation_val) {
                    cash_flows[m] = immediate_payoff;
                }
                }
            }
        }
        double total_cash_flow = 0.0;
        for (std::size_t m = 0; m < num_sims; ++m) {
            double terminal_payoff = payoff(mat[m][num_steps]);
            total_cash_flow += std::max(cash_flows[m], terminal_payoff);
        }
        double mean_cash_flow = total_cash_flow / static_cast<double>(num_sims);
        return factor_discount * mean_cash_flow;
        

    }
};



#endif