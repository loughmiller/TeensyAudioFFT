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
  float maxDecibels = 60;
  float minDecibels = 200;


  for (uint16_t i=3; i<FFT_SIZE; i++) {
    float decibels = 20.0*log10(magnitudes[i]);
    maxDecibels = max(decibels, maxDecibels);

    // Serial.print(i);
    // Serial.print(" = ");
    // Serial.print(decibels);
    // Serial.print(" | ");
    // Serial.print(minDecibels);
    // Serial.print(" - ");
    // Serial.println(maxDecibels);
  }

  maxDecibels = max(this->currentMax, maxDecibels);
  // min can not be easily collected, often gets very low values
  // so minDecibels gets used
  // should use 5th & 95th percentiles as min / max
  minDecibels = maxDecibels/2.0;

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

  for (uint16_t i=3; i<FFT_SIZE; i++) {
    float decibels = 20.0*log10(magnitudes[i]);
    float relativeIntesity = decibels - this->movingMin;
    relativeIntesity = relativeIntesity < 0.0 ? 0.0 : relativeIntesity;
    relativeIntesity /= (this->movingMax - this->movingMin);
    relativeIntesity = relativeIntesity > 1.0 ? 1.0 : relativeIntesity;
    this->intensities[i-3] = relativeIntesity;
    // Serial.print(i);
    // Serial.print(" | ");
    // Serial.println(relativeIntesity);
  }
}

void TeensyAudioFFT::loop() {
  if (samplingIsDone()) {
    updateMagnitudes();
    samplingBegin();
  }
}
