#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "reg.hpp"

TEST(RegTest, BasisFunctionsEvaluation) {
    double x = 100.0;
    double spot = 100.0;

    double p0 = phi0(x, spot);
    double p1 = phi1(x, spot);
    double p2 = phi2(x, spot);

    EXPECT_NEAR(p0, std::exp(-0.5), 1e-5);
    EXPECT_NEAR(p1, std::exp(-0.5) * 0.0, 1e-5); 
    EXPECT_FALSE(std::isnan(p2));
}

TEST(RegTest, RegressionAndPredictionReturnsCoefficients) {

    double spot = 100.0;
    std::vector<double> x = {90.0, 100.0, 110.0};
    std::vector<double> y = {10.0, 5.0, 1.0};

    std::vector<double> coeffs = reg(x, y, spot);
    double prediction = predict(coeffs, 100.0, spot);

    ASSERT_EQ(coeffs.size(), 3);
    EXPECT_FALSE(std::isnan(coeffs[0]));
    EXPECT_FALSE(std::isnan(coeffs[1]));
    EXPECT_FALSE(std::isnan(coeffs[2]));
    EXPECT_FALSE(std::isnan(prediction));
}