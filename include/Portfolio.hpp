#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
#include "ModelAssetModel.hpp"
struct Underlying {
    std::string ticker;
    double spot_price;
    double volatility;
    int model_column_index;
    
};
struct Position {
    std::shared_ptr<Underlying> underlying; 
    std::shared_ptr<Payoff> payoff;         
    double quantity;
    double maturity;
    double contract_multiplier;
};


class Portfolio {
private:
    std::vector<Position> positions_;
    
public:
    void addPosition(const Position& pos){
        positions_.push_back(pos);
    }
    double get_max_maturity()const{
        double maturity=0;
        for (std::size_t j=0; j<positions_.size();++j){
            if (positions_[j].maturity>maturity)
            maturity=positions_[j].maturity;
        }
        return maturity;
    }
    ///doesn't work with bermudian & US contracts
    template <typename StochasticModel> 
    std::vector<double> evaluate_portfolio_risk(StochasticModel& model,std::size_t num_steps_max,int num_simulations,double risk_free_rate,std::mt19937_64& rng) const {
        double t_max =get_max_maturity();
        std::vector<double> positions_values(num_simulations, 0.0);
        for (std::size_t sim=0; sim<num_simulations;++sim){
            double PnL_sim=0.0;
            std::vector<std::vector<double>> price=model.simulate_paths(t_max,num_steps_max,rng);
            for (std::size_t j=0; j<positions_.size();++j){
                double maturity_j=positions_[j].maturity;
                std::size_t index_j=positions_[j].underlying->model_column_index;
                std::vector<double> price_j_i;
                std::size_t pos_steps = std::min(static_cast<std::size_t>((maturity_j / t_max) * num_steps_max),num_steps_max);
                for (std::size_t k=0; k<=pos_steps;++k){
                    price_j_i.push_back(price[k][index_j]);
                }
                double raw_payoff = (*positions_[j].payoff)(price_j_i);
                double discounted_val = raw_payoff * positions_[j].quantity * positions_[j].contract_multiplier * std::exp(-risk_free_rate * maturity_j);
                PnL_sim += discounted_val;
            }
            portfolio_values[sim]=PnL_sim;
        }
    return portfolio_values;
    }
};

#endif