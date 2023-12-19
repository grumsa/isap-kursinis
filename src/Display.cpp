#include "Display.hpp"

/* VARIABLES */
byte Display::custom_char_arr[8][8] = {               // special characters for the lcd to display spectrum
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F}, // Level 1
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F}, // Level 2
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F}, // Level 3
    {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F}, // Level 4
    {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // Level 5
    {0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // Level 6
    {0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // Level 7
    {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}  // Level 8 (Full)
};

/* FUNCTION DECLARATIONS */
void createCustomChars (void);
void displaySpectrum (double* binned_frequencies_arr, int number_of_bins);
void frequencyBinning (double* magnitude, double* binned_frequencies_arr, int number_of_bins, int sample_size);

/* FUNCTION DEFINITIONS */
/**
 * @brief Display class constructor
 */
Display::Display(LiquidCrystal_I2C &lcd_instance) : lcd(lcd_instance) {}

/**
 * @brief creates the predefined custom chars and writes them into the lcd memory
 */
void Display::createCustomChars() {
    for (int i = 0; i < 8; i++) {
        lcd.createChar(i, custom_char_arr[i]);
    }
}

/**
 * @brief calculates the average magnitude for frequency bins from a given magnitude array,
 * @note reduces the frequency data resolution
 */
void Display::frequencyBinning(double* magnitude_arr, double* binned_frequencies_arr, int number_of_bins, int sample_size) {
    int bin_size = sample_size / 2 / number_of_bins; 

    for (int i = 0; i < number_of_bins; i++) {
        double bin_average = 0;
        for (int j = 0; j < bin_size; j++) {
            bin_average += magnitude_arr[i * bin_size + j];
        }
        binned_frequencies_arr[i] = bin_average / bin_size;
    }
}

/**
 * @brief display the spectrum on the lcd, 
 * @note unfortunately, this is currently hardcoded for the 16x2 lcd displays with 8x5 char matrices
 */
void Display::displaySpectrum(double* binned_frequencies_arr, int number_of_bins) {
    lcd.clear();
    for (int i = 0; i < number_of_bins; i++) {
         int scaled_value = (int)map(binned_frequencies_arr[i], 0, 100, 0, 16);

        int lower_value = (uint8_t)0x20;
        int upper_value = (uint8_t)0x20;

        if (scaled_value <= 8) {
            lower_value = scaled_value;
            if (lower_value > 7) {
                lower_value = 7;
            }
        } else {
            lower_value = 7;
            upper_value = scaled_value - 8;
            if (upper_value > 7) {
                upper_value = 7;
            }
        }

        lcd.setCursor(i, 1);
        lcd.write(lower_value);
        lcd.setCursor(i, 0);
        lcd.write(upper_value);
    }
}