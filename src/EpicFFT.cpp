/** @file EpicFFT.hpp */

#include "EpicFFT.hpp"

/* FUNCTION DECLARATIONS */
int  log2Floor (int n);
void swap (double *x, double *y);
void reverseBits (double* data, int size);
void FFT (double *real_arr, double *imag_arr, int samples);
void hammingWindow (double *data, int size);
void convertToMagnitude  (double *real_arr, double *imag_arr, int samples);

/* FUNCTION DEFINITIONS */
/**
 * @brief finds the floor of the base-2 logarithm of a positive integer
 */
int EpicFFT::log2Floor(int n) {
    int retval = 0;
    while (n > 1) {
        n /= 2;
        retval++;
    }
    return retval;
}

/**
 * @brief swaps the values of two double pointers
 */
void EpicFFT::swap(double *x, double *y) {
    double temp = *x;
    *x = *y;
    *y = temp;
}

/**
 * @brief applies hamming window to an array
 */
void EpicFFT::hammingWindow(double *data, int size) {
    for (int i = 0; i < size; i++) {
        double hamming = 0.54 - 0.46 * cos(2.0 * M_PI * i / (size - 1)); // Hamming window
        data[i] *= hamming;
    }
}

/**
 * @brief reverses bits in an array
 */
void EpicFFT::reverseBits(double* data, int size) {
    int j = 0;
    for (int i = 0; i < size - 1; ++i) {
        if (i < j) {
            swap(&data[i], &data[j]);
        }
        int m = size >> 1;
        while (m >= 2 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
}

/**
 * @brief the almighty Fast Fourier Transform
 *
 * Uses the Cooley-Tukey radix-2 cecimation in time (DIT) 
 * algorithm to perform the transformation.
 *
 * @param real_arr  - pointer to real data array
 * @param imag_arr  - pointer to imaginary data array
 * @param samples   - sample count
 */
void EpicFFT::FFT(double *real_arr, double *imag_arr, int samples) {
    reverseBits(real_arr, samples); // Reverse bits

    for (int l = 0; l < log2Floor(samples); l++) {
        int l1 = 1 << l; // 2^l
        int l2 = l1 << 1; // 2^(l+1)

        for (int j = 0; j < l1; j++) {
            // twiddle factors
            double angle = -2 * PI * j / l2;
            double c1 = cos(angle);
            double c2 = sin(angle);

            for (int i = j; i < samples; i += l2) {
                int i1 = i + l1;
                double t1 = c1 * real_arr[i1] - c2 * imag_arr[i1];
                double t2 = c1 * imag_arr[i1] + c2 * real_arr[i1];

                real_arr[i1] = real_arr[i] - t1;
                imag_arr[i1] = imag_arr[i] - t2;
                real_arr[i] += t1;
                imag_arr[i] += t2;
            }
        }
    }
}

/**
 * @brief computes the magnitude of complex FFT results
 * @note converts them back into real_arr
 */
void EpicFFT::convertToMagnitude(double *real_arr, double *imag_arr, int samples) {
  for (int i = 0; i < samples; i++) {
    real_arr[i] = sqrt((real_arr[i] * real_arr[i]) + (imag_arr[i] * imag_arr[i]));
  }
}