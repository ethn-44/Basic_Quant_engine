#ifndef PATHPAYOFF_HPP
#define PATHPAYOFF_HPP
#include <vector> 
#include <cmath>
#include <algorithm>

class PathPayoff{
    public:
    virtual ~PathPayoff() = default;
    virtual double operator()(const std::vector<double>& path) const = 0;
};

class AsianCallPayoff: public PathPayoff{
    private:
        double strike_;

    public:
        explicit AsianCallPayoff(double strike): strike_(strike){}

        double operator()(const std::vector<double>& path) const override{
            double mn=0;
            std::size_t n=path.size();
            for (std::size_t i=1; i<n; ++i){
                mn+=path[i];
            }
            return std::max(mn/static_cast<double>(n-1)-strike_,0.0);
        }
};
class AsianPutPayoff: public PathPayoff{
    private:
        double strike_;

    public:
        explicit AsianPutPayoff(double strike): strike_(strike){}

        double operator()(const std::vector<double>& path) const override{
            double mn=0;
            std::size_t n=path.size();
            for (std::size_t i=1; i<n; ++i){
                mn+=path[i];
            }
            return std::max(strike_-mn/static_cast<double>(n-1),0.0);
        }
};
////////////////////////////////////BARRIER/////////////////////////
class BarrierUpAndOutCallPayoff : public PathPayoff {
private:
    double strike_;
    double barrier_;

public:
    BarrierUpAndOutCallPayoff(double strike, double barrier)
        : strike_(strike), barrier_(barrier) {}

    double operator()(const std::vector<double>& path) const override {
        std::size_t n=path.size();
        for (std::size_t i=1;i<n;++i){
            if (path[i]>=barrier_)
                return 0;
        }
        return std::max(path.back()-strike_,0.0);
    }
};
class BarrierUpAndOutPutPayoff : public PathPayoff {
private:
    double strike_;
    double barrier_;

public:
    BarrierUpAndOutPutPayoff(double strike, double barrier)
        : strike_(strike), barrier_(barrier) {}

    double operator()(const std::vector<double>& path) const override {
        std::size_t n=path.size();
        for (std::size_t i=1;i<n;++i){
            if (path[i]>=barrier_)
                return 0;
        }
        return std::max(strike_-path.back(),0.0);
    }
};

class BarrierDownAndOutCallPayoff : public PathPayoff {
private:
    double strike_;
    double barrier_;

public:
    BarrierDownAndOutCallPayoff(double strike, double barrier)
        : strike_(strike), barrier_(barrier) {}

    double operator()(const std::vector<double>& path) const override {
        std::size_t n=path.size();
        for (std::size_t i=1;i<n;++i){
            if (path[i]<=barrier_)
                return 0;
        }
        return std::max(path.back()-strike_,0.0);
    }
};

class BarrierDownAndOutPutPayoff : public PathPayoff {
private:
    double strike_;
    double barrier_;

public:
    BarrierDownAndOutPutPayoff(double strike, double barrier)
        : strike_(strike), barrier_(barrier) {}

    double operator()(const std::vector<double>& path) const override {
        std::size_t n=path.size();
        for (std::size_t i=1;i<n;++i){
            if (path[i]<=barrier_)
                return 0;
        }
        return std::max(strike_ -path.back(),0.0);
    }
};


#endif