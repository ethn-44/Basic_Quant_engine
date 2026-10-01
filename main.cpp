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
#include "Calib_cost.hpp"
#include "Nelder_Meadsolver.hpp"
using std::cout;
using std::endl;
template <typename Params>
inline void print_results(const Params& calibrated_params, double final_cost, double elapsed_sec) {
    std::cout << "\n--- FIN DE LA CALIBRATION (" << elapsed_sec << "s) ---\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Volatilite 1 : " << calibrated_params[0] << "\n";
    std::cout << "Volatilite 2 : " << calibrated_params[1] << "\n";
    std::cout << "Correlation  : " << calibrated_params[2] << "\n";
    std::cout << "Cout final   : " << final_cost << "\n";
}
int main() {
    // 1. Instanciation des dependances reelles
    std::vector<double> spots = {100.0, 100.0};
    std::vector<double> vols_init = {0.20, 0.20};
    double rate = 0.03;
    std::vector<std::vector<double>> cov = {{0.04, 0.01}, {0.01, 0.04}};

    MultiAsset model(spots, vols_init, rate, cov);
    MonteCarloEngine mc_engine;
    MultiAssetsPayoff payoff({false, false}, false);

    // 2. Donnees de marche de test
    std::vector<double> market_prices = {10.5, 8.2};
    std::vector<double> strike_list = {1.0, 1.05};
    std::vector<double> barrier_list = {-1.0, -1.0};
    std::vector<bool> is_put_list = {false, false};
    std::vector<double> maturity_list = {1.0, 1.0};

    // 3. Vrai objet de calibration
    CalibrationObjective cost_fn(model, mc_engine, payoff, market_prices, spots, 
                                 2000, 50, strike_list, barrier_list, 
                                 is_put_list, maturity_list, rate);

    // 4. Solveur Nelder-Mead
    NelderMeadSolver solver(1.0, 2.0, 0.5, 0.5, 1e-4, 100);
    std::vector<double> initial_params = {0.15, 0.15, 0.20}; // [vol1, vol2, rho]

    std::cout << "--- DEBUT DE LA CALIBRATION ---" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> calibrated_params = solver.solve(cost_fn, initial_params, 0.80);
    auto end = std::chrono::high_resolution_clock::now();
    
    double elapsed = std::chrono::duration<double>(end - start).count();
    double final_cost = cost_fn(calibrated_params);

    print_results(calibrated_params, final_cost, elapsed);

    return 0;
}