#include <cmath>
#include "Portfolio.hpp"
#include <random>
#include <iostream>

void Portfolio::addPosition(const Position& pos){
    positions_.push_back(pos);
}
template <typename StochasticModel>
std::vector<double> Portfolio::evaluate_portfolio_risk(const Portfolio& portfolio,StochasticModel& model,int num_simulations,double confidence_level)const{
    std::vector<double> pnl_product(positions_.size());
    std::vector<double> vec;
    for (std::size_t i=0; i<positions_.size();++i){
        comment je récup les underlying différent puis calcule les différents payoff. 
    }
}