#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "Risk_calc.hpp"

TEST(RiskCalcTest, CalculateRiskMetricsReturnsCorrectValues) {

    std::vector<double> values = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
    double confidence_level = 0.90; 

    RiskMetrics metrics = calculate_risk_metrics(values, confidence_level);

    EXPECT_FALSE(std::isnan(metrics.mean));
    EXPECT_FALSE(std::isnan(metrics.variance));
    EXPECT_FALSE(std::isnan(metrics.skewness));
    EXPECT_FALSE(std::isnan(metrics.var));
    EXPECT_FALSE(std::isnan(metrics.cvar));

    EXPECT_NEAR(metrics.mean, 55.0, 1e-5);
}