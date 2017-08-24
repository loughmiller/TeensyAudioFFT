#include "TeensyAudioFFT.h"

TeensyAudioFFT::TeensyAudioFFT () {
  this->alpha = 0.35;
  this->movingMax = 80;
  this->movingMin = 40;
  this->currentMax = 0;
  this->currentMin = 1000000000000;
  this->sampleInterval = 1000;
  this->lastSampleTime = 0;
}

void TeensyAudioFFT::updateRelativeIntensities(uint32_t currentTime) {
  float intensities[FFT_SIZE];
  float maxDecibels = 50; // max floor of 40
  float minDecibels = 200;
  float minFloor = 40;

  for (uint16_t i=1; i<FFT_SIZE; i++) {
    float decibels = 20.0*log10(magnitudes[i]);
    maxDecibels = max(decibels, maxDecibels);
    minDecibels = min(decibels, minDecibels);
    minDecibels = max(minFloor, minDecibels);

    // Serial.print(i);
    // Serial.print(" = ");
    // Serial.print(decibels);
    // Serial.print(" | ");
    // Serial.print(minDecibels);
    // Serial.print(" - ");
    // Serial.println(maxDecibels);
  }

  maxDecibels = max(this->currentMax, maxDecibels);
  minDecibels = min(this->currentMin, minDecibels);

  // Serial.print(minDecibels);
  // Serial.print(" - ");
  // Serial.println(maxDecibels);

  if (currentTime > (this->lastSampleTime + this->sampleInterval)) {
    this->lastSampleTime = currentTime;
    this->movingMax = (this->alpha * maxDecibels) +
      ((1 - this->alpha) * this->movingMax);
    this->movingMin = (this->alpha * minDecibels) +
      ((1 - this->alpha) * this->movingMin);

    // reset min/max
    this->currentMax = 0;
    this->currentMin = 1000000000000;
  }

  for (uint16_t i=0; i<FFT_SIZE; i++) {
    float decibels = 20.0*log10(magnitudes[i]);
    float relativeIntesity = decibels - this->movingMin;
    relativeIntesity = relativeIntesity < 0.0 ? 0.0 : relativeIntesity;
    relativeIntesity /= (this->movingMax - this->movingMin);
    relativeIntesity = relativeIntesity > 1.0 ? 1.0 : relativeIntesity;
    this->intensities[i] = relativeIntesity;
  }
}

void TeensyAudioFFT::loop() {
  if (samplingIsDone()) {
    updateMagnitudes();
    samplingBegin();
  }
}
