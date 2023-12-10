#ifndef DSP_H
#define DSP_H

#include <complex>
#include <vector>

namespace DSP
{
    // function to create a Hann window of length N
    std::vector<std::complex<double>> WindowHann(int N);

    // function to create a discrete fourier transform
    std::vector<std::complex<double>> dft(std::vector<std::complex<double>> X);

}

#endif