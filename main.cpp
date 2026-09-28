#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include "Payoff.hpp"
#include "BlackScholes.hpp"
#include <numeric>
#include <random>
#include "MonteCarlo.hpp"
#include "Greeks.hpp"
#include "PathPayoff.hpp"
#include "Heston.hpp"
#include "Merton.hpp"
using std::cout;
using std::endl;


template <typename Func>
auto measure_execution(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return std::make_pair(result, elapsed.count());
}

int main() {
    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.05;
    double vol = 0.20;
    double maturity = 1.0;
    std::size_t num_sims = 50000;
    std::size_t num_steps = 252;

    MonteCarloEngine mc_engine(42);
    GreeksEngine greeks_engine(mc_engine);

    PayoffPut put_payoff(strike);

    std::cout << "=== 1. TEST BLACK-SCHOLES & GREEQUES ===" << std::endl;
    BlackScholesModel bs_model(spot, rate, vol);
    
    double bs_price = mc_engine.price_path_dependant(bs_model, put_payoff, maturity, num_sims, num_steps); // ou price_bs
    std::cout << "Prix Put Européen (BS) : " << bs_price << std::endl;

    GreeksResult bs_greeks = greeks_engine.calculate(bs_model, put_payoff, maturity, num_sims);
    std::cout << "Delta : " << bs_greeks.delta << " | Gamma : " << bs_greeks.gamma 
              << " | Vega : " << bs_greeks.vega << " | Rho : " << bs_greeks.rho << std::endl;

    std::cout << "\n=== 2. TEST MODELE DE MERTON (JUMP-DIFFUSION) ===" << std::endl;
    
    MertonModel merton_model(spot, rate, vol, 1.0, -0.10, 0.15);
    mc_engine.export_paths_to_csv("simulation_paths.csv", merton_model, 1.0, 252, 50);
    mc_engine.reset_seed();
    double merton_price = mc_engine.price_path_dependant(merton_model, put_payoff, maturity, num_sims, num_steps);
    std::cout << "Prix Put Merton : " << merton_price << std::endl;

    GreeksResult merton_greeks = greeks_engine.calculate(merton_model, put_payoff, maturity, num_sims);
    std::cout << "Delta Merton : " << merton_greeks.delta << " | Vega Merton : " << merton_greeks.vega << std::endl;

    std::cout << "\n=== 3. TEST MODELE DE HESTON (VOL STOCHASTIQUE) ===" << std::endl;
    
    HestonModel heston_model(spot, rate, 0.04, 2.0, 0.04, 0.3, -0.7);
    
    mc_engine.reset_seed();
    double heston_price = mc_engine.price_path_dependant(heston_model, put_payoff, maturity, num_sims, num_steps);
    std::cout << "Prix Put Heston : " << heston_price << std::endl;

    GreeksResult heston_greeks = greeks_engine.calculate(heston_model, put_payoff, maturity, num_sims);
    std::cout << "Delta Heston : " << heston_greeks.delta << " | Vega Heston : " << heston_greeks.vega << std::endl;

    return 0;
}
