#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP
struct Underlying {
    std::string ticker;
    double spot_price;
    double volatility;
    
};

struct Position {
    std::shared_ptr<Underlying> underlying; 
    std::shared_ptr<Payoff> payoff;         
    double quantity;
    double contract_multiplier;
    bool is_leveraged = false;
    double margin_requirement = 1.0;
};


class Portfolio {
private:
    std::vector<Position> positions_;

public:
    void addPosition(const Position& pos);
    std::vector<double> evaluate(const MonteCarloEngine& engine) const;
};

#endif