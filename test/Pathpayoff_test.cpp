#include <gtest/gtest.h>
#include <vector>
#include "PathPayoff.hpp"

TEST(PathPayoffTest, AsianCallPayoffCalculatesAverageCorrectly) {
    AsianCallPayoff asianCall(100.0);
    std::vector<double> path = {100.0, 110.0, 120.0}; 

    double result = asianCall(path);

    EXPECT_NEAR(result, 15.0, 1e-5);
}

TEST(PathPayoffTest, BarrierUpAndOutCallTriggersKnockOut) {
    
    BarrierUpAndOutCallPayoff barrierCall(100.0, 130.0);
    
    std::vector<double> path = {100.0, 110.0, 135.0, 105.0};

    double result = barrierCall(path);

    
    EXPECT_EQ(result, 0.0);
}

TEST(PathPayoffTest, BarrierUpAndOutCallSurvivesAndPaysOff) {
    BarrierUpAndOutCallPayoff barrierCall(100.0, 140.0);
    std::vector<double> path = {100.0, 110.0, 130.0, 120.0};
    
    double result = barrierCall(path);

    EXPECT_NEAR(result, 20.0, 1e-5);
}