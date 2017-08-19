#include <Arduino.h>
// #include <algorithm>
#include "TeensyAudioFFTCore.h"

class TeensyAudioFFT {
  private :
    float alpha;
    float movingMax;
    float movingMin;
    float currentMax;
    float currentMin;
    uint16_t sampleInterval;
    uint32_t lastSampleTime;
    float spectrum[FFT_SIZE];

  public :
    float intensities[FFT_SIZE];
    TeensyAudioFFT();
    void loop();
    void updateRelativeIntensities(uint32_t currentTime);
};
