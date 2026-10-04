#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "Heston.hpp"

TEST(HestonModelTest, ConstructorAndGetters) {
    HestonModel model(100.0, 0.05, 0.04, 2.0, 0.04, 0.3, -0.7);
    EXPECT_EQ(model.spot(), 100.0);
    EXPECT_EQ(model.rate(), 0.05);
    EXPECT_EQ(model.vol(), 0.04);
    EXPECT_EQ(model.kappa(), 2.0);
    EXPECT_EQ(model.theta(), 0.04);
    EXPECT_EQ(model.xi(), 0.3);
    EXPECT_EQ(model.rho(), -0.7);
}

TEST(HestonModelTest, WithMethodsReturnNewModifiedModel) {
    HestonModel model(100.0, 0.05, 0.04, 2.0, 0.04, 0.3, -0.7);
    
    auto m_spot = model.with_spot(105.0);
    EXPECT_EQ(m_spot.spot(), 105.0);
    EXPECT_EQ(m_spot.rate(), 0.05);

    auto m_rate = model.with_rate(0.08);
    EXPECT_EQ(m_rate.rate(), 0.08);
    EXPECT_EQ(m_rate.vol(), 0.04);

    auto m_vol = model.with_vol(0.09);
    EXPECT_EQ(m_vol.vol(), 0.09);
    EXPECT_EQ(m_vol.spot(), 100.0);
}

TEST(HestonModelTest, GeneratePathSizeAndInitialValue) {
    HestonModel model(100.0, 0.05, 0.04, 2.0, 0.04, 0.3, -0.7);
    std::mt19937_64 rng(42);
    std::size_t num_steps = 20;
    double maturity = 1.0;

    std::vector<double> path = model.generate_path(maturity, num_steps, rng);

    EXPECT_EQ(path.size(), num_steps + 1);
    EXPECT_EQ(path[0], 100.0);
}