#ifndef HESTON_HPP
#define HESTON_HPP
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

class HestonModel{
    private :
    double spot_;
    double rate_;
    double vol_;
    double kappa_;
    double theta_;
    double xi_;
    double rho_;

    public :
    explicit HestonModel(double spot,double rate,double vol,double kappa, double theta, double xi, double rho) : spot_(spot),rate_(rate),vol_(vol),kappa_(kappa),theta_(theta),xi_(xi),rho_(rho){}
    double spot() const { return spot_; }
    double rate() const { return rate_; }
    double vol() const { return vol_; }
    double kappa() const { return kappa_; }
    double theta() const { return theta_; }
    double xi() const { return xi_; }
    double rho() const { return rho_; }
    HestonModel with_spot(double new_spot) const {
        return HestonModel(new_spot, rate_, vol_, kappa_, theta_, xi_, rho_);
    }
    HestonModel with_vol(double new_vol) const { // Modifie vol_ pour le Vega
        return HestonModel(spot_, rate_, new_vol, kappa_, theta_, xi_, rho_);
    }
    HestonModel with_rate(double new_rate) const {
        return HestonModel(spot_, new_rate, vol_, kappa_, theta_, xi_, rho_);
    }
    template <typename Generator>
    std::vector<double> generate_path(double maturity, std::size_t num_steps, Generator& gen) const{
        const double dt = maturity/static_cast<double>(num_steps);
        const double sqdt=std::sqrt(dt); 
        const double  rhocomp=std::sqrt(1.0-rho_*rho_);
        std::normal_distribution<double> dist(0.0, 1.0);
        double v=vol_;
        std::vector<double> vol(num_steps+1);
        vol[0]=v;
        std::vector<double> S(num_steps+1);
        S[0]=spot_;
        for (std::size_t i=0; i<num_steps; ++i){
            double Z_1=dist(gen);
            double Z_2=dist(gen);
            double Z_v=rho_*Z_1+rhocomp*Z_2;
            double Z_s=Z_1;
            double v_plus = std::max(v, 0.0);
            vol[i+1]=vol[i]+kappa_*(theta_-v_plus)*dt+xi_*std::sqrt(v_plus)*sqdt*Z_v;
            S[i+1]=S[i]*std::exp((rate_-v_plus/2.0)*dt+std::sqrt(v_plus)*sqdt*Z_s);
            v = vol[i+1];
        }
        return S;
    }
};

#endif