#ifndef GREEKS_HPP
#define GREEKS_HPP
#include <random>
#include "MonteCarlo.hpp"

struct GreeksResult{
    double price;
    double delta;
    double gamma;
    double vega;
    double rho;
};

class GreeksEngine{
    private:
    MonteCarloEngine& engine_;
    double h_ratio_;
    ///
    public :
    explicit GreeksEngine(MonteCarloEngine& engine, double h_ratio = 0.01): engine_(engine), h_ratio_(h_ratio) {}
    template <typename PayoffType>
    GreeksResult calculate(const BlackScholesModel& model,const PayoffType& payoff,double maturity,std::size_t num_sims){
        GreeksResult greek;
        const double S0 = model.spot();
        const double h = S0 * h_ratio_;
        const double h_vol = 0.01;
        const double h_r = 0.0001;
        ///V(S_0), V(S_0-h),V(S_0+h) : Delta & Gamma
        BlackScholesModel model_up = model.with_spot(S0 + h);
        BlackScholesModel model_down = model.with_spot(S0 - h);
        ///
        engine_.reset_seed();
        double v_base = engine_.price(model, payoff, maturity, num_sims);
        engine_.reset_seed();
        double v_up = engine_.price(model_up, payoff, maturity, num_sims);
        engine_.reset_seed();
        double v_down = engine_.price(model_down, payoff, maturity, num_sims);
        greek.price=v_base;
        ///Delta & Gamma
        greek.delta = (v_up-v_down)/(2*h);
        greek.gamma = (v_up+v_down-2*v_base)/(h*h);
        //Vega
        double vol=model.vol();
        BlackScholesModel model_up_vol = model.with_vol(vol + h_vol);
        BlackScholesModel model_down_vol = model.with_vol(vol - h_vol);
        engine_.reset_seed();
        double v_up_vol = engine_.price(model_up_vol, payoff, maturity, num_sims);
        engine_.reset_seed();
        double v_down_vol = engine_.price(model_down_vol, payoff, maturity, num_sims);
        greek.vega=(v_up_vol-v_down_vol)/(2*h_vol);
        //rho
        BlackScholesModel model_r_up   = model.with_rate(model.rate() + h_r);
        BlackScholesModel model_r_down = model.with_rate(model.rate() - h_r);
        engine_.reset_seed();
        double v_r_up = engine_.price(model_r_up, payoff, maturity, num_sims);
        engine_.reset_seed();
        double v_r_down = engine_.price(model_r_down, payoff, maturity, num_sims);
        greek.rho=(v_r_up - v_r_down) / (2.0 * h_r);
        return greek;
    }
};
















#endif