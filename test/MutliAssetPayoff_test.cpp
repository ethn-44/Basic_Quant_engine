#include <gtest/gtest.h>
#include <vector>
#include "MultiAssetPayoff.hpp"

TEST(MultiAssetsPayoffTest, EvaluateBasketReturnsCorrectPayoff) {
    std::vector<bool> is_asian = {false, false};
    bool is_american = false;
    MultiAssetsPayoff payoffEngine(is_asian, is_american);

    std::vector<std::vector<double>> prices = {
        {100.0, 200.0}, 
        {110.0, 220.0}  
    };
    std::vector<double> weights = {0.5, 0.5};
    double strike = 1.0; 

    double result = payoffEngine.evaluate_basket(prices, weights, strike);

    EXPECT_NEAR(result, 0.1, 1e-5);
}

TEST(MultiAssetsPayoffTest, EvaluateRainbowCallReturnsCorrectPayoff) {

    std::vector<bool> is_asian = {false, false};
    bool is_american = false;
    MultiAssetsPayoff payoffEngine(is_asian, is_american);

    std::vector<std::vector<double>> prices = {
        {100.0, 200.0},
        {120.0, 180.0} 
    };
    double barrier = 1.0;
    double strike = 1.0;
    bool is_put = false;

    double result = payoffEngine.evaluate_rainbow(prices, barrier, strike, is_put);

    EXPECT_NEAR(result, 0.2, 1e-5);
}