#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "MultiAssetModel.hpp"

TEST(MultiAssetTest, ConstructorAndGetters) {
    std::vector<double> spots = {100.0, 150.0};
    std::vector<double> vol = {0.2, 0.25};
    double rate = 0.05;
    std::vector<std::vector<double>> cov = {
        {1.0, 0.5},
        {0.5, 1.0}
    };
    specific_params params;

    MultiAsset multiAsset(spots, vol, rate, cov, params);
    EXPECT_EQ(multiAsset.rate(), 0.05);
}

TEST(MultiAssetTest, PriceAssetsBsReturnsValidMatrix) {
    std::vector<double> spots = {100.0, 150.0};
    std::vector<double> vol = {0.2, 0.25};
    double rate = 0.05;
    std::vector<std::vector<double>> cov = {
        {1.0, 0.3},
        {0.3, 1.0}
    };
    specific_params params;

    MultiAsset multiAsset(spots, vol, rate, cov, params);
    std::mt19937_64 rng(42);
    double maturity = 1.0;
    std::size_t num_steps = 10;

    auto paths = multiAsset.price_Assets_bs(maturity, num_steps, rng);

    
    EXPECT_EQ(paths.size(), num_steps + 1);
    
    EXPECT_EQ(paths[0].size(), 2);
    EXPECT_EQ(paths[0][0], 100.0);
    EXPECT_EQ(paths[0][1], 150.0);
}