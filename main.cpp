#include <iostream>
#include <vector>
#include "Payoff.hpp"
#include "BlackScholes.hpp"
#include <numeric>
#include <random>
#include "MonteCarlo.hpp"
using std::cout;
using std::endl;

int main() {
    double spot=105.0;
    double rate=0.1;
    double vol=0.3;
    double maturity=0.25;
    std::size_t num_sims=100000000; //num_sims est forcément pair
    PayoffCall call(100.0);
    BlackScholesModel bsmodel(spot,rate,vol);
    MonteCarloEngine mcengine(42);
    double price = mcengine.price(bsmodel,call,maturity,num_sims);
    cout<<price<<endl;
    return 0;
}
