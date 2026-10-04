#include <gtest/gtest.h>
#include <vector>
#include "Payoff.hpp"

TEST(PayoffCallTest, SpotAboveStrikeReturnsPositivePayoff) {
    PayoffCall call(100.0);
    double spot = 115.0;
    double result = call(spot);
    EXPECT_EQ(result, 15.0);
}

TEST(PayoffCallTest, SpotBelowStrikeReturnsZero) {
    PayoffCall call(100.0);
    double spot = 90.0;
    double result = call(spot);
    EXPECT_EQ(result, 0.0);
}

TEST(PayoffCallTest, PathBasedOperatorUsesLastElement) {
    PayoffCall call(100.0);
    std::vector<double> path = {95.0, 98.0, 105.0};
    double result = call(path);
    EXPECT_EQ(result, 5.0);
}

// --- Tests pour PayoffPut ---

TEST(PayoffPutTest, SpotBelowStrikeReturnsPositivePayoff) {
    PayoffPut put(100.0);
    double spot = 85.0;
    double result = put(spot);
    EXPECT_EQ(result, 15.0);
}

TEST(PayoffPutTest, SpotAboveStrikeReturnsZero) {
    PayoffPut put(100.0);
    double spot = 110.0;
    double result = put(spot);
    EXPECT_EQ(result, 0.0);
}

TEST(PayoffPutTest, PathBasedOperatorUsesLastElement) {
    PayoffPut put(100.0);
    std::vector<double> path = {102.0, 99.0, 90.0};
    double result = put(path);
    EXPECT_EQ(result, 10.0);
}