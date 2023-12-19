/** @file Display.hpp */

#pragma once

#include "Common.hpp"

class Display {
    private:
        LiquidCrystal_I2C &lcd;
        static byte custom_char_arr[8][8];
    public:
        Display(LiquidCrystal_I2C &lcd_instance);

        void    createCustomChars  (void);
        void    displaySpectrum    (double* binned_frequencies_arr, int number_of_bins);
        void    frequencyBinning   (double* magnitude, double* binned_frequencies_arr, int number_of_bins, int sample_size);
};