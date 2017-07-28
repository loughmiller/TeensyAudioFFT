// TeensyAudioFFT example
//
// Simple example for the TeensyAudioFFT Library
// This code is heavily based on Tony DiCola's guide:
// http://learn.adafruit.com/fft-fun-with-fourier-transforms/
// his code: https://github.com/tdicola/adafruit_guide_fft

#include <Arduino.h>
#include <FastLED.h>
#include <TeensyAudioFFT.h>

const int AUDIO_INPUT_PIN = 14;         // Input ADC pin for audio data.

const int POWER_LED_PIN = 13;

const int LED_DATA_PIN = 3;
const int LED_COUNT = 45;

CRGB leds[LED_COUNT];

// Setup
void setup() {
  // Setup serial port.
  Serial.begin(38400);

  // Setup ADC and audio input.
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);

  // Setup LEDs
  FastLED.setBrightness(32);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, LED_COUNT).setCorrection( Typical8mmPixel );;

  // Turn on the power indicator LED.
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);

  // Start sampling audio
  setAudioInputPin(AUDIO_INPUT_PIN);
  samplingBegin();
}

void loop() {

  for (int i=0; i<LED_COUNT; i++) {
    leds[i] = 0x000000;
  }

  float intensity = readIntensity(2, 3);

  int intensityCount = (int)(intensity * (float)LED_COUNT);

  // Serial.println(intensityCount);

  for (int i=0; i<intensityCount; i++) {
    leds[i] = 0x0000FF;
  }

  FastLED.show();
}
