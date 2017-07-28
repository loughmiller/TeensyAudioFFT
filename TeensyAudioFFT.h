// This code is heavily based on Tony DiCola's guide: http://learn.adafruit.com/fft-fun-with-fourier-transforms/
// https://github.com/tdicola/adafruit_guide_fft
#ifndef TEENSY_AUDIO_FFT_H
#define TEENSY_AUDIO_FFT_H

#define ARM_MATH_CM4
#include <arm_math.h>
#include <Arduino.h>

#define SAMPLE_RATE_HZ 9000              // Sample rate of the audio in hertz.
#define SPECTRUM_MIN_DB 40.0           // Audio intensity (in decibels) that maps to low LED brightness.
#define SPECTRUM_MAX_DB 70.0           // Audio intensity (in decibels) that maps to high LED brightness.

#define FFT_SIZE 256              // Size of the FFT.  Realistically can only be at most 256
#define SAMPLE_SIZE FFT_SIZE*2     // Complex FFT functions require a coefficient for the imaginary part of the
                                        // input.  This makes the sample array 2x the FFT_SIZE
#define ANALOG_READ_RESOLUTION 10  // Bits of resolution for the ADC.
#define ANALOG_READ_AVERAGING 16   // Number of samples to average with each ADC reading.

extern IntervalTimer samplingTimer;
extern float samples[FFT_SIZE*2];
extern float magnitudes[FFT_SIZE];
extern int sampleCounter;
extern int audioInputPin;

extern float readIntensity(uint16_t lowBin, uint16_t highBin);
extern void samplingBegin();
extern void updateMagnitudes();
extern void setAudioInputPin(int pin);
extern void samplingCallback();
extern bool samplingIsDone();
extern void windowMean(float* magnitudes,
  int lowBin,
  int highBin,
  float* windowMean,
  float* otherMean);
extern int frequencyToBin(float frequency);

#endif
