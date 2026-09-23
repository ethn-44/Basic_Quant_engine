#include <iostream>
#include <vector>
#include "Payoff.hpp"
#include "BlackScholes.hpp"
#include <numeric>
#include <random>
#include "MonteCarlo.hpp"
#include "Greeks.hpp"
using std::cout;
using std::endl;

int main() {
    const double spot = 100.0;       // S0
    const double strike = 100.0;     // K (At-The-Money)
    const double rate = 0.05;        // r = 5%
    const double vol = 0.20;         // sigma = 20%
    const double maturity = 1.0;     // T = 1 an
    const std::size_t num_sims = 10'000'000; // 10 millions de simulations

    // 2. Instanciation des objets principaux
    BlackScholesModel model(spot, rate, vol);
    CallPayoff payoff(strike);
    MonteCarloEngine mc_engine(42); // Seed = 42
    GreeksEngine greeks_engine(mc_engine, 0.01); // Bump ratio = 1%

    std::cout << "==========================================" << std::endl;
    std::cout << "   MONTE CARLO ENGINE - RISK ANALYTICS    " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Simulations : " << num_sims << std::endl;
    std::cout << "Spot        : " << spot << std::endl;
    std::cout << "Strike      : " << strike << std::endl;
    std::cout << "Maturite    : " << maturity << " an(s)" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // 3. Mesure du temps d'exécution et calcul des Grecques
    auto start_time = std::chrono::high_resolution_clock::now();

    GreeksResult result = greeks_engine.calculate(model, payoff, maturity, num_sims);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 4. Affichage des résultats
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Prix Option : " << result.price << std::endl;
    std::cout << "Delta       : " << result.delta << std::endl;
    std::cout << "Gamma       : " << result.gamma << std::endl;
    std::cout << "Vega        : " << result.vega  << std::endl;
    std::cout << "Rho         : " << result.rho   << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Temps d'execution : " << elapsed.count() << " s" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
