#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "MertonModel.hpp"

TEST(MertonModelTest, ConstructorAndGetters) {
    MertonModel model(100.0, 0.05, 0.2, 0.75, -0.1, 0.2);
    EXPECT_EQ(model.spot(), 100.0);
    EXPECT_EQ(model.rate(), 0.05);
    EXPECT_EQ(model.vol(), 0.2);
    EXPECT_EQ(model.lambda(), 0.75);
    EXPECT_EQ(model.mu_j(), -0.1);
    EXPECT_EQ(model.delta_j(), 0.2);
}

TEST(MertonModelTest, WithMethodsReturnNewModifiedModel) {
    MertonModel model(100.0, 0.05, 0.2, 0.75, -0.1, 0.2);
    
    auto m_spot = model.with_spot(110.0);
    EXPECT_EQ(m_spot.spot(), 110.0);
    EXPECT_EQ(m_spot.rate(), 0.05);

    auto m_rate = model.with_rate(0.08);
    EXPECT_EQ(m_rate.rate(), 0.08);
    EXPECT_EQ(m_rate.vol(), 0.2);

    auto m_vol = model.with_vol(0.25);
    EXPECT_EQ(m_vol.vol(), 0.25);
    EXPECT_EQ(m_vol.spot(), 100.0);
}

TEST(MertonModelTest, GeneratePathSizeAndInitialValue) {
    MertonModel model(100.0, 0.05, 0.2, 0.75, -0.1, 0.2);
    std::mt19937_64 rng(42);
    std::size_t num_steps = 15;
    double maturity = 1.0;

    std::vector<double> path = model.generate_path(maturity, num_steps, rng);

    EXPECT_EQ(path.size(), num_steps + 1);
    EXPECT_EQ(path[0], 100.0);
}