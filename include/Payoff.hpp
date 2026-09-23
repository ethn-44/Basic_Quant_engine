#ifndef PAYOFF_HPP
#define PAYOFF_HPP
#include <algorithm>
class Payoff{
    public:
    virtual ~Payoff() = default;
    virtual double operator()(double spot) const = 0;
};
class PayoffCall : public Payoff {
    private :
    double strike_;
    public :
    explicit PayoffCall(double strike) : strike_(strike) {}

    double operator()(double spot) const override {
        return std::max(spot-strike_,0.0);
    }

};
class PayoffPut : public Payoff {
    private :
    double strike_;
    public : 
    explicit PayoffPut(double strike) : strike_(strike){}

    double operator()(double spot) const override {
        return std::max(strike_-spot,0.0);
    }
};
#endif