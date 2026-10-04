#include <gtest/gtest.h>
#include <memory>
#include "Greeks.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "Payoff.hpp"

TEST(GreeksEngineTest, CalculateReturnsValidGreeksStructure) {

    MonteCarloEngine mcEngine(42); 
    GreeksEngine greeksEngine(mcEngine, 0.025, 10);
    
    BlackScholesModel model(100.0, 0.05, 0.2);
    PayoffCall payoff(100.0);
    
    double maturity = 1.0;
    std::size_t num_sims = 1000; 

    GreeksResult result = greeksEngine.calculate(model, payoff, maturity, num_sims);
    EXPECT_FALSE(std::isnan(result.price));
    EXPECT_FALSE(std::isnan(result.delta));
    EXPECT_FALSE(std::isnan(result.gamma));
    EXPECT_FALSE(std::isnan(result.vega));
    EXPECT_FALSE(std::isnan(result.rho));

    EXPECT_GE(result.delta, 0.0);
    EXPECT_LE(result.delta, 1.0);
}