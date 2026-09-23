#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP
#include <random>
#include <cstddef>
#include "BlackScholes.hpp"
#include "Payoff.hpp"
#include <cmath>
class MonteCarloEngine{
    private:
    std::mt19937_64 gen_;
    public:
    explicit MonteCarloEngine(int seed=42): gen_(seed){}
    template <typename PayoffType>
    double price(const BlackScholesModel& model, const PayoffType& payoff, double maturity, std::size_t num_sims){ //num_sims est pair
        std::normal_distribution<double> dist(0.0,1.0);
        double c=0.0;
        const double r = model.rate();
        const double sigma = model.vol();
        const double drift = (r - 0.5 * sigma * sigma) * maturity;
        const double diffusion = sigma * std::sqrt(maturity);
        const double discount_factor = std::exp(-r * maturity);
        for (std::size_t i = 0; i < num_sims/2; i++){
            double normal=dist(gen_);
            double S_t_1=model.spot_at_maturity(drift,diffusion,normal);
            double S_t_2=model.spot_at_maturity(drift,diffusion,-normal);
            c+=payoff(S_t_1)+payoff(S_t_2);
        }
        return discount_factor * (c/static_cast<double>(num_sims));
    }
};



#endif