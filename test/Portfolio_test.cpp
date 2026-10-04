#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <random>
#include "Portfolio.hpp"
#include "Payoff.hpp"

class MockModel {
public:
    std::vector<std::vector<double>> simulate_paths([[maybe_unused]] double maturity, [[maybe_unused]] std::size_t num_steps, [[maybe_unused]] std::mt19937_64& rng) const {

        return {
            {100.0},
            {110.0}
        };
    }
};

TEST(PortfolioTest, AddPositionAndGetMaxMaturity) {

    Portfolio portfolio;
    auto underlying = std::make_shared<Underlying>();
    underlying->ticker = "AAPL";
    underlying->spot_price = 100.0;
    underlying->volatility = 0.2;
    underlying->model_column_index = 0;

    auto payoff = std::make_shared<PayoffCall>(100.0);

    Position pos1{underlying, payoff, 10.0, 0.5, 1.0};
    Position pos2{underlying, payoff, 5.0, 1.0, 1.0};


    portfolio.addPosition(pos1);
    portfolio.addPosition(pos2);


    EXPECT_NEAR(portfolio.get_max_maturity(), 1.0, 1e-5);
}

TEST(PortfolioTest, EvaluatePortfolioRiskReturnsValues) {

    Portfolio portfolio;
    auto underlying = std::make_shared<Underlying>();
    underlying->ticker = "AAPL";
    underlying->spot_price = 100.0;
    underlying->volatility = 0.2;
    underlying->model_column_index = 0;

    auto payoff = std::make_shared<PayoffCall>(100.0);
    Position pos{underlying, payoff, 1.0, 1.0, 1.0};
    portfolio.addPosition(pos);

    MockModel model;
    std::mt19937_64 rng(42);
    std::size_t num_simulations = 10;
    std::size_t num_steps_max = 2;
    double risk_free_rate = 0.05;


    std::vector<double> portfolio_values = portfolio.evaluate_portfolio_risk(model, num_steps_max, num_simulations, risk_free_rate, rng);
    ASSERT_EQ(portfolio_values.size(), num_simulations);
    EXPECT_FALSE(std::isnan(portfolio_values[0]));
}