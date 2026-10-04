#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "MultiAssetModel.hpp"
#include "Payoff.hpp"
#include "MonteCarlo.hpp"
#include "PathPayoff.hpp"
#include "Portfolio.hpp"
#include "Risk_calc.hpp"

void run_quant_engine_pipeline() {

    std::vector<double> spots = {100.0, 150.0};
    std::vector<double> vols = {0.20, 0.25};
    double rate = 0.03;
    std::vector<std::vector<double>> cov_matrix = {
        {0.04, 0.01},
        {0.01, 0.06}
    };
    specific_params params; 

    MultiAsset model(spots, vols, rate, cov_matrix, params);

    auto und_aapl = std::make_shared<Underlying>(Underlying{"AAPL", 100.0, 0.20, 0});
    auto und_goog = std::make_shared<Underlying>(Underlying{"GOOG", 150.0, 0.25, 1});

    auto european_call = std::make_shared<PayoffCall>(105.0);
    auto path_payoff = std::make_shared<AsianCallPayoff>(100.0); 

    Portfolio portfolio;
    portfolio.addPosition(Position{
        und_aapl,
        european_call,
        10.0,   
        1.0,    
        1.0     
    });

    portfolio.addPosition(Position{
        und_goog,
        path_payoff,
        5.0,
        1.0,
        1.0
    });


    std::mt19937_64 rng(42);
    std::size_t num_steps = 50;
    int num_simulations = 2000;

    std::vector<double> pnl_values = portfolio.evaluate_portfolio_risk(
        model, num_steps, num_simulations, rate, rng
    );

    RiskMetrics metrics = calculate_risk_metrics(pnl_values, 0.99);

    std::cout << "=== Portfolio :  ===" << std::endl;
    std::cout << "Portfolio Max Maturity: " << portfolio.get_max_maturity() << std::endl;
    std::cout << "VaR (99%):              " << metrics.var << std::endl;
    std::cout << "Expected Shortfall:     " << metrics.cvar << std::endl;
    std::cout << "Portfolio Mean PnL:     " << metrics.mean << std::endl;

    BlackScholesModel single_model(spots[0], vols[0], rate); 
    double maturity = 1.0;
    std::size_t num_paths_to_export = 200;
    MonteCarloEngine mc_engine;
    mc_engine.export_paths_to_csv("paths.csv", single_model, maturity, num_steps, num_paths_to_export);
    std::cout << "Monte Carlo paths successfully exported to paths.csv" << std::endl;
}
int main() {
    run_quant_engine_pipeline();
    return 0;
}