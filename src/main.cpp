/** @file main.cpp */

/* INCLUDES */
#include "Common.hpp"
#include "Display.hpp"
#include "EpicFFT.hpp"

/* DEBUGS */
// #define DEBUG_SIGNAL_READING
// #define DEBUG_WINDOWING
// #define DEBUG_FFT_OUTPUT
// #define DEBUG_MAGNITUDE

/* DEFINES */
#define MICROPHONE_PIN A0
#define NUMBER_OF_BINS 16
#define SAMPLES 128       // Must be a power of 2
#define SAMPLING_FREQUENCY 1800 // ~ max for my board apparently

/* VARIABLES */
double real_arr[SAMPLES];
double imag_arr[SAMPLES];
double bin_freqs_arr[NUMBER_OF_BINS];
LiquidCrystal_I2C lcd(0x27, 16, 2); // default address for PCF8574 adapter
Display disp(lcd);
EpicFFT fft;

/* FUNCTION DECLARES */
void    setup  (void);
void    loop   (void);

/* FUNCTION DEFINITIONS */
/**
 * @brief setup for hardware
 */
void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    disp.createCustomChars();
}

/**
 * @brief main loop
 */
#if !defined(DEBUG_SIGNAL_READING) && !defined(DEBUG_WINDOWING) && !defined(DEBUG_FFT_OUTPUT) && !defined(DEBUG_MAGNITUDE)
void loop() {
    // 1. read the signal into real values array
    double sum = 0;
    for (int i = 0; i < SAMPLES; i++) {
        real_arr[i] = analogRead(MICROPHONE_PIN);
        sum += real_arr[i];
        imag_arr[i] = 0;
        delayMicroseconds(1000000 / SAMPLING_FREQUENCY); //for high precision
    }
    double average = sum / SAMPLES;
    // subtract DC offset from each sample
    for (int i = 0; i < SAMPLES; i++) {
        real_arr[i] -= average;
    }

    // 2. apply window function
    fft.hammingWindow(real_arr, SAMPLES);

    // 3. compute FFT
    fft.FFT(real_arr, imag_arr, SAMPLES);

    // 4. calculate magnitude
    fft.convertToMagnitude(real_arr, imag_arr, SAMPLES);

    // 5. frequency binning
    disp.frequencyBinning(real_arr, bin_freqs_arr, NUMBER_OF_BINS, SAMPLES);

    // 6. display spectrum
    disp.displaySpectrum(bin_freqs_arr, NUMBER_OF_BINS);

}
#endif //!defined(DEBUG_SIGNAL_READING) && !defined(DEBUG_WINDOWING) && !defined(DEBUG_FFT_OUTPUT) && !defined(DEBUG_MAGNITUDE)


/**
 * @brief main loop for conditional debugging
 */
#if defined(DEBUG_SIGNAL_READING) || defined(DEBUG_WINDOWING) || defined(DEBUG_FFT_OUTPUT) || defined(DEBUG_MAGNITUDE)
void loop() {
    double sum = 0;
    for (int i = 0; i < SAMPLES; i++) {
        real_arr[i] = analogRead(MICROPHONE_PIN);
        sum += real_arr[i];
        imag_arr[i] = 0;
        delayMicroseconds(1000000 / SAMPLING_FREQUENCY); //for high precision
    }
    double average = sum / SAMPLES;
    // Subtract DC offset from each sample
    for (int i = 0; i < SAMPLES; i++) {
        real_arr[i] -= average;
    }

    #ifdef DEBUG_SIGNAL_READING
    Serial.println("Before Windowing:");
    for (int i = 0; i < SAMPLES; i++) {
        Serial.print("Signal[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(real_arr[i]);
    }
    #endif

    fft.hammingWindow(real_arr, SAMPLES);

    #ifdef DEBUG_WINDOWING
    Serial.println("After Windowing:");
    for (int i = 0; i < SAMPLES; i++) {
        Serial.print("Windowed Signal[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(real_arr[i]);
    }
    #endif

    fft.FFT(real_arr, imag_arr, SAMPLES);
    
    #ifdef DEBUG_FFT_OUTPUT
    Serial.println("FFT Output:");
    for (int i = 0; i < 10; i++) {  // Adjust the number of printed values as needed
        Serial.print("FFT[");
        Serial.print(i);
        Serial.print("] - Real: ");
        Serial.print(real_arr[i]);
        Serial.print(", Imaginary: ");
        Serial.println(imag_arr[i]);
    }
    #endif

    fft.convertToMagnitude(real_arr, imag_arr, SAMPLES);

    #ifdef DEBUG_MAGNITUDE
    Serial.println("Magnitude of FFT Output:");
    for (int i = 0; i < SAMPLES; i++) {
        Serial.print("Magnitude[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(real_arr[i]);
    }
    #endif

    disp.frequencyBinning(real_arr, bin_freqs_arr, NUMBER_OF_BINS, SAMPLES);

    disp.displaySpectrum(bin_freqs_arr, NUMBER_OF_BINS);

    delay(1000); // debug only
}
#endif //defined(DEBUG_SIGNAL_READING) || defined(DEBUG_WINDOWING) || defined(DEBUG_FFT_OUTPUT) || defined(DEBUG_MAGNITUDE)