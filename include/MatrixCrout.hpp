#ifndef MATRIXCROUT_HPP
#define MATRIXCROUT_HPP

#include <cmath>
#include <vector>
#include <cstddef>

struct decomposition{
    std::vector<std::vector<double>> D;
    std::vector<std::vector<double>> L;
};

inline decomposition Crout_decompo(const std::vector<std::vector<double>>& A){
    std::size_t n = A.size();
    decomposition Crout_dec;
    Crout_dec.D = std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0));
    Crout_dec.L = std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0));
    Crout_dec.D[0][0]=A[0][0];
    Crout_dec.L[0][0] = 1.0;
    for (std::size_t i = 1; i < n; ++i) {
        Crout_dec.L[i][i]=1.0;
        for (std::size_t j = 0; j <= i; ++j) {
            double c=0;
            for (std::size_t k=0; k<j ;++k)
                c+=Crout_dec.D[k][k]*Crout_dec.L[i][k]*Crout_dec.L[j][k];
            if (i == j) {
                Crout_dec.D[i][i]=A[i][i]- c;
            } else {
               Crout_dec.L[i][j]=(A[i][j]-c)/Crout_dec.D[j][j];
            }
        }
    }
    return Crout_dec;
}
#endif