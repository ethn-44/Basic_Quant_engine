#ifndef REG_HPP
#define REG_HPP
#include <vector>
#include <cmath>

inline std::vector<double> reg(const std::vector<double>& x, const std::vector<double>& y) {
    std::size_t n=x.size();
    double M_0= static_cast<double>(n);
    double M_1=0 , M_2=0,M_3=0,M_4=0;
    double Y_1=0, Y_2=0,Y_3=0;
    #pragma omp parallel for reduction(+:M_1, M_2, M_3, M_4, Y_1, Y_2, Y_3) schedule(static)
    for (std::size_t i=0; i<n;++i){
        double xi2 = x[i] * x[i];
        M_1+=x[i];
        M_2+=xi2;
        M_3+=xi2*x[i];
        M_4+=xi2*xi2;
        Y_1+=y[i];
        Y_2+=x[i]*y[i];
        Y_3+=xi2*y[i];
    }   
    double det_M=M_0*(M_2 * M_4 - M_3 * M_3)- M_1 * (M_1 * M_4 - M_3 * M_2) + M_2 * (M_1 * M_3 - M_2 * M_2);
    if (std::abs(det_M) < 1e-12) {
        return {0.0, 0.0, 0.0};
    }
    double inv_det = 1.0 / det_M;
    double a0=Y_1* (M_2 * M_4 - M_3 * M_3) - M_1   * (Y_2 * M_4 - Y_3 * M_3) + M_2   * (Y_2 * M_3 - Y_3 * M_2);
    double a1=M_0* (Y_2 * M_4 - Y_3 * M_3) - Y_1 * (M_1 * M_4 - M_3 * M_2)  + M_2   * (M_1 * Y_3 - Y_2 * M_2);
    double a2 = M_0   * (M_2 * Y_3 - M_3 * Y_2) - M_1 * (M_1 * Y_3 - M_3 * Y_1) + Y_1   * (M_1 * M_3 - M_2 * M_2);

    return {a0*inv_det,a1*inv_det,a2*inv_det};
}

inline double predict(const std::vector<double>& a, const double& x){
    return a[0] + x * (a[1] + x * a[2]);
}   

#endif