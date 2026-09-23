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
    double spot() const { return spot_; }
    double rate() const { return rate_; }
    double vol() const { return vol_; }
    inline double spot_at_maturity(double drift, double diffusion, double normal) const {
        return spot_ * std::exp(drift + diffusion * normal);
    }
    BlackScholesModel with_spot(double new_spot) const {
        return BlackScholesModel(new_spot, rate_, vol_);
    }
    BlackScholesModel with_vol(double new_vol) const {
        return BlackScholesModel(spot_, rate_, new_vol);
    }
    BlackScholesModel with_rate(double new_rate) const {
        return BlackScholesModel(spot_, new_rate, vol_);
    }
};
#endif