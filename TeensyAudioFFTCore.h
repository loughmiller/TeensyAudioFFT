// This code is heavily based on Tony DiCola's guide: http://learn.adafruit.com/fft-fun-with-fourier-transforms/
// https://github.com/tdicola/adafruit_guide_fft
#ifndef TEENSY_AUDIO_FFT_H
#define TEENSY_AUDIO_FFT_H

#define ARM_MATH_CM4
#include <arm_math.h>
#include <Arduino.h>

#define SAMPLE_RATE_HZ 6400          // Sample rate of the audio in hertz.
#define FFT_SIZE 256                 // Size of the FFT.  Realistically can only be at most 256
                                     // 25Hz per bin

#define SAMPLE_SIZE FFT_SIZE*2       // Complex FFT functions require a coefficient for the imaginary part of the
                                     // input.  This makes the sample array 2x the FFT_SIZE
#define ANALOG_READ_RESOLUTION 10    // Bits of resolution for the ADC.
#define ANALOG_READ_AVERAGING 16     // Number of samples to average with each ADC reading.


// general state
extern IntervalTimer samplingTimer;
extern float samples[FFT_SIZE*2];
extern float magnitudes[FFT_SIZE];
extern uint16_t sampleCounter;
extern uint8_t audioInputPin;

// stuff for the moving average
extern uint16_t decibleSampleInterval;
extern uint32_t lastDecibleSampleTime;
extern uint8_t maxDecibles;
extern uint8_t minDecibles;
extern uint8_t movingAvgMaxDecibles;
extern uint8_t movingAvgMinDecibles;
extern float movingAvgAlpha;

// functions
extern void TeensyAudioFFTSetup(uint8_t audioInputPin);
float readRelativeIntensity(uint32_t currentTime, uint8_t lowBin, uint8_t highBin);
extern float readDecibels(uint16_t lowBin, uint16_t highBin);
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
