#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP
#include <cmath>


class BlackScholesModel{
    private:
    double spot_;
    double rate_;
    double vol_;
    
    public :
    explicit BlackScholesModel(double spot,double rate,double vol) : spot_(spot),rate_(rate),vol_(vol){}

    double rate() const { return rate_; }
    double vol() const { return vol_; }
    inline double spot_at_maturity(double drift, double diffusion, double normal) const {
        return spot_ * std::exp(drift + diffusion * normal);
    }
};
#endif