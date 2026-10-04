#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "MatrixCrout.hpp"

TEST(MatrixCroutTest, DecompositionOfSimple2x2Matrix) {
    std::vector<std::vector<double>> A = {
        {4.0, 2.0},
        {2.0, 3.0}
    };

    decomposition result = Crout_decompo(A);

    ASSERT_EQ(result.D.size(), 2);
    ASSERT_EQ(result.L.size(), 2);

    EXPECT_DOUBLE_EQ(result.D[0][0], 4.0);
    EXPECT_DOUBLE_EQ(result.L[1][0], 0.5);
    EXPECT_DOUBLE_EQ(result.D[1][1], 2.0);
    EXPECT_DOUBLE_EQ(result.L[0][0], 1.0);
    EXPECT_DOUBLE_EQ(result.L[1][1], 1.0);
}