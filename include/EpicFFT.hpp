/** @file EpicFFT.hpp */

#include "Common.hpp"

#pragma once

class EpicFFT {
    private:
        static int  log2Floor           (int n);
        static void swap                (double *x, double *y);
        static void reverseBits         (double* data, int size);
    public:
        void        FFT                 (double *real_arr, double *imag_arr, int samples);
        void        hammingWindow       (double *data, int size);
        void        convertToMagnitude  (double *real_arr, double *imag_arr, int samples);
};