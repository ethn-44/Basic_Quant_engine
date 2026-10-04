#include <gtest/gtest.h>
#include <vector>
#include "Nelder_Meadsolver.hpp"

class DummyObjective : public CalibrationObjective {
public:
    double operator()(const std::vector<double>& params) const override {
        if (params.size() < 2) return 0.0;
        double x = params[0];
        double y = params[1];
        return (x - 2.0) * (x - 2.0) + (y - 3.0) * (y - 3.0);
    }
};

TEST(NelderMeadSolverTest, ReflectCalculatesCorrectPoint) {
    NelderMeadSolver solver;
    std::vector<double> centroid = {1.0, 1.0};
    std::vector<double> worst = {3.0, 3.0};
    double alpha = 1.0;

    std::vector<double> reflected = solver.reflect(centroid, worst, alpha);

    EXPECT_NEAR(reflected[0], -1.0, 1e-5);
    EXPECT_NEAR(reflected[1], -1.0, 1e-5);
}

TEST(NelderMeadSolverTest, SortSimplexOrdersByCost) {
    NelderMeadSolver solver;

    std::vector<std::vector<double>> simplex = {
        {1.0, 1.0, 10.0},
        {0.0, 0.0, 2.0},
        {2.0, 2.0, 5.0}
    };

    solver.sort_simplex(simplex);

    EXPECT_NEAR(simplex[0].back(), 2.0, 1e-5);
    EXPECT_NEAR(simplex[1].back(), 5.0, 1e-5);
    EXPECT_NEAR(simplex[2].back(), 10.0, 1e-5);
}