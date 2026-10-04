#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>
#include "BlackScholesModel.hpp"

TEST(BlackScholesModelTest, ConstructorAndGetters) {
    BlackScholesModel model(100.0, 0.05, 0.2);
    EXPECT_EQ(model.spot(), 100.0);
    EXPECT_EQ(model.rate(), 0.05);
    EXPECT_EQ(model.vol(), 0.2);
}

TEST(BlackScholesModelTest, SpotAtMaturityCalculation) {
    BlackScholesModel model(100.0, 0.05, 0.2);
    double drift = 0.01;
    double diffusion = 0.02;
    double normal = 1.5;
    double expected = 100.0 * std::exp(0.01 + 0.02 * 1.5);
    EXPECT_DOUBLE_EQ(model.spot_at_maturity(drift, diffusion, normal), expected);
}

TEST(BlackScholesModelTest, WithMethodsReturnNewModifiedModel) {
    BlackScholesModel model(100.0, 0.05, 0.2);
    
    auto m_spot = model.with_spot(110.0);
    EXPECT_EQ(m_spot.spot(), 110.0);
    EXPECT_EQ(m_spot.rate(), 0.05);
    EXPECT_EQ(m_spot.vol(), 0.2);

    auto m_rate = model.with_rate(0.08);
    EXPECT_EQ(m_rate.spot(), 100.0);
    EXPECT_EQ(m_rate.rate(), 0.08);
    EXPECT_EQ(m_rate.vol(), 0.2);

    auto m_vol = model.with_vol(0.3);
    EXPECT_EQ(m_vol.spot(), 100.0);
    EXPECT_EQ(m_vol.rate(), 0.05);
    EXPECT_EQ(m_vol.vol(), 0.3);
}

TEST(BlackScholesModelTest, GeneratePathSizeAndInitialValue) {
    BlackScholesModel model(100.0, 0.05, 0.2);
    std::mt19937_64 rng(42); 
    std::size_t num_steps = 10;
    double maturity = 1.0;

    std::vector<double> path = model.generate_path(maturity, num_steps, rng);

    EXPECT_EQ(path.size(), num_steps + 1);

    EXPECT_EQ(path[0], 100.0);
}