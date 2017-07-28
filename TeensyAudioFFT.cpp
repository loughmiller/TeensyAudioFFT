// This code is heavily based on Tony DiCola's guide: http://learn.adafruit.com/fft-fun-with-fourier-transforms/
// https://github.com/tdicola/adafruit_guide_fft

#include "TeensyAudioFFT.h"

IntervalTimer samplingTimer;
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
int sampleCounter;
int audioInputPin;

////////////////////////////////////////////////////////////////////////////////
// READ INTENSITY
////////////////////////////////////////////////////////////////////////////////

// Bin 0 is the magnitude or the whole thing according to documentation
// Bin 1 starts at 0Hz
// Bin size can be determined by sampleRateHz/FFT_size
// Example of 9000Hz / 256 = 35.15Hz,
// so bin 1 is 0Hz to 35.15Hz, bin 2 is 35.15Hz to 70.30Hz
// lowBin and highBin are inclusive
float readIntensity(uint16_t lowBin, uint16_t highBin) {
  // See if there is new data available, if so, run the calcs
  if (samplingIsDone()) {
    updateMagnitudes();
    samplingBegin();
  }

  float intensity, otherMean;

  windowMean(magnitudes, lowBin, highBin, &intensity, &otherMean);

  // Convert intensity to decibels.
  intensity = 20.0*log10(intensity);
  // Scale the intensity and clamp between 0 and 1.0.
  intensity -= SPECTRUM_MIN_DB;
  intensity = intensity < 0.0 ? 0.0 : intensity;
  intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
  return intensity > 1.0 ? 1.0 : intensity;
}

////////////////////////////////////////////////////////////////////////////////
// THE WHOLE FFT THING
////////////////////////////////////////////////////////////////////////////////
void updateMagnitudes() {
  // Run FFT on sample data.
  arm_cfft_radix4_instance_f32 fft_inst;
  arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
  arm_cfft_radix4_f32(&fft_inst, samples);
  // Calculate magnitude of complex numbers output by the FFT.
  arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
// SAMPLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(audioInputPin);
  // Complex FFT functions require a coefficient for the imaginary part of the
  // input.  Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

bool samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}

////////////////////////////////////////////////////////////////////////////////
// UTILITY FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void setAudioInputPin(int pin) {
  audioInputPin = pin;
}

// Compute the average magnitude of a target frequency window vs. all other
// frequencies.
void windowMean(float* magnitudes,
  int lowBin,
  int highBin,
  float* windowMean,
  float* otherMean) {
    *windowMean = 0;
    *otherMean = 0;
    // Notice the first magnitude bin is skipped because it represents the
    // average power of the signal.
    for (int i = 1; i < FFT_SIZE/2; ++i) {
      if (i >= lowBin && i <= highBin) {
        *windowMean += magnitudes[i];
      }
      else {
        *otherMean += magnitudes[i];
      }
    }
    *windowMean /= (highBin - lowBin) + 1;
    *otherMean /= (FFT_SIZE / 2 - (highBin - lowBin));
}

// Convert a frequency to the appropriate FFT bin it will fall within.
int frequencyToBin(float frequency) {
  float binFrequency = float(SAMPLE_RATE_HZ) / float(FFT_SIZE);
  return int(frequency / binFrequency);
}
