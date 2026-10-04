#include <gtest/gtest.h>
#include <vector>
#include "Calib_cost.hpp"
#include "MultiAssetModel.hpp"
#include "MultiAssetPayoff.hpp"
#include "MonteCarlo.hpp"

TEST(CalibrationObjectiveTest, OperatorReturnsPenaltyForInvalidConstraints) {

    std::vector<double> spots = {100.0, 150.0};
    std::vector<double> vol = {0.2, 0.25};
    double rate = 0.05;
    std::vector<std::vector<double>> cov = {{1.0, 0.3}, {0.3, 1.0}};
    specific_params spec_params;
    MultiAsset model(spots, vol, rate, cov, spec_params);

    MonteCarloEngine mc_engine(42);
    std::vector<bool> is_asian = {false, false};
    MultiAssetsPayoff payoff(is_asian, false);

    std::vector<double> market_prices = {10.0};
    std::vector<double> spot_prices = {100.0, 150.0};
    std::size_t num_simulations = 100;
    std::size_t num_steps = 10;
    std::vector<double> strike_list = {100.0};
    std::vector<double> barrier_list = {1.0};
    std::vector<bool> is_put_list = {false};
    std::vector<double> maturity_list = {1.0};
    double risk = 0.01;

    CalibrationObjective calibObj(model, mc_engine, payoff, market_prices, spot_prices,
                                  num_simulations, num_steps, strike_list, barrier_list,
                                  is_put_list, maturity_list, risk);

    std::vector<double> invalid_params = {-0.2, 0.25, 1.5};


    double cost = calibObj(invalid_params);

    EXPECT_EQ(cost, 1e10);
}