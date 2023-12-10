#include "dsp.h"
#include <math.h>

std::vector<std::complex<double>> DSP::WindowHann(int N)
{
    // Create a vector object for the output
    std::vector<std::complex<double>> output;

    // Populate this with the window values
    double ND = static_cast<double>(N);
    for(int n = 0; n<N; n++)
    {
        double nD = static_cast<double>(n);
        double realPart = 0.5 * (1.0 - cos((2.0 *M_PI * nD) /ND));
        output.push_back(std::complex<double>(realPart, 0.0));
    }
    return output;
}

std::vector<std::complex<double>> DSP::dft(std::vector<std::complex<double>> X)
{
    // Determine number of samples. 
    int N = X.size();
    int K = N;

    // Allocate memory for internals
    std::complex<double> intSum;

    // Allocate memory for output
    std::vector<std::complex<double>> output;
    output.reserve(K);

    // Loop through each k
    for (int k = 0; k < K; k++)
    {
        // loop through each n 
        intSum = std::complex<double>(0,0);
        for (int n = 0; n<N; n++)
        {
            double realPart = cos(((2*M_PI)/N) * k * n);
            double imagPart = sin(((2*M_PI)/N) * k * n);
            
            std::complex<double> w (realPart, -imagPart);
            intSum += X[n] * w;
        }
        output.push_back(intSum);
    }

    return output;
}