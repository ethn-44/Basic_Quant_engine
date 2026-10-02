#include <cmath>
#include "Portfolio.hpp"
#include <random>
#include <iostream>

void Portfolio::addPosition(const Position& pos){
    positions_.push_back(pos)
}
std::vector<double> Portfolio::evaluate(const MonteCarloEngine& engine) const