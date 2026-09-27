#ifndef REG_HPP
#define REG_HPP
#include <vector>
#include <cmath>
#include<algorithm>
#include <omp.h>
double phi0(double x,double initial_spot){
    return std::exp(-x/(2.0*initial_spot));
}
double phi1(double x,double initial_spot){
    return std::exp(-x/(2.0*initial_spot))*(1.0-x/initial_spot);
}
double phi2(double x,double initial_spot){
    return std::exp(-x/(2.0*initial_spot))*(1.0-2.0*x/initial_spot+ (x/initial_spot)*(x/initial_spot)/2.0);
} 

inline std::vector<double> reg(const std::vector<double>& x, const std::vector<double>& y,double initial_spot) {
    std::size_t n=x.size();
    double m00 = 0.0, m01 = 0.0, m02 = 0.0;
    double m11 = 0.0, m12 = 0.0, m22 = 0.0;
    double y0 = 0.0, y1 = 0.0, y2 = 0.0;
    #pragma omp parallel for reduction(+:m00, m01, m02, m11, m12, m22, y0, y1, y2) schedule(static)
    for (std::size_t i=0; i<n;++i){
        double p0 = phi0(x[i], initial_spot);
        double p1 = phi1(x[i], initial_spot);
        double p2 = phi2(x[i], initial_spot);
        double y_val = y[i];
        m00 += p0 * p0;
        m01 += p0 * p1;
        m02 += p0 * p2;
        m11 += p1 * p1;
        m12 += p1 * p2;
        m22 += p2 * p2;
        y0 += p0 * y_val;
        y1 += p1 * y_val;
        y2 += p2 * y_val;
    }   
    double det_M = m00 * (m11 * m22 - m12 * m12)- m01 * (m01 * m22 - m12 * m02)+ m02 * (m01 * m12 - m11 * m02);
    if (std::abs(det_M) < 1e-12) {
        return {0.0, 0.0, 0.0};
    }
    double inv_det = 1.0 / det_M;
    double a0 = (y0 * (m11 * m22 - m12 * m12) - m01 * (y1 * m22 - m12 * y2) + m02 * (y1 * m12 - m11 * y2)) * inv_det;
    double a1 = (m00 * (y1 * m22 - m12 * y2) - y0 * (m01 * m22 - m12 * m02) + m02 * (m01 * y2 - y1 * m02)) * inv_det;
    double a2 = (m00 * (m11 * y2 - m12 * y1) - m01 * (m01 * y2 - m12 * y0) + y0 * (m01 * m12 - m11 * m02)) * inv_det;

    return {a0, a1, a2};
}

inline double predict(const std::vector<double>& a, const double& x, double initial_spot){
    return a[0] * phi0(x, initial_spot) + a[1] * phi1(x, initial_spot) + a[2] * phi2(x, initial_spot);;
} 
   

#endif