#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <fstream>
#include "MonteCarlo.hpp"
#include "BlackScholes.hpp"
#include "Payoff.hpp"

TEST(MonteCarloEngineTest, PriceBlackScholesReturnsValidPrice) {

    MonteCarloEngine engine(42);
    BlackScholesModel model(100.0, 0.05, 0.2);
    PayoffCall payoff(100.0);
    double maturity = 1.0;
    std::size_t num_sims = 1000; 
    double price = engine.price_bs(model, payoff, maturity, num_sims);

    EXPECT_FALSE(std::isnan(price));
    EXPECT_GT(price, 0.0);
    EXPECT_LT(price, 50.0);
}

TEST(MonteCarloEngineTest, PricePathDependentReturnsValidPrice) {

    MonteCarloEngine engine(42);
    BlackScholesModel model(100.0, 0.05, 0.2);
    PayoffCall payoff(100.0);
    double maturity = 1.0;
    std::size_t num_sims = 500;
    std::size_t num_steps = 50;


    double price = engine.price_path_dependant(model, payoff, maturity, num_sims, num_steps);


    EXPECT_FALSE(std::isnan(price));
    EXPECT_GE(price, 0.0);
}

TEST(MonteCarloEngineTest, ExportPathsToCsvCreatesValidFile) {

    MonteCarloEngine engine(42);
    BlackScholesModel model(100.0, 0.05, 0.2);
    std::string filename = "test_paths.csv";
    double maturity = 1.0;
    std::size_t num_steps = 10;
    std::size_t num_paths = 5;

    engine.export_paths_to_csv(filename, model, maturity, num_steps, num_paths);

    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open());
    file.close();
    std::remove(filename.c_str());
}