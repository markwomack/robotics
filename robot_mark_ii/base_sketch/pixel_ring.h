//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PIXEL_RING_H
#define PIXEL_RING_H

// Arduino includes
#include <inttypes.h>

// Third party includes
#include <Adafruit_NeoPixel.h>

enum RingState {
  RING_OFF,
  RING_GREEN_CW,
  RING_RED_CW,
  RING_BLUE_CW,
  RING_WHITE_FADE
};

struct RingColor {
  char red;
  char green;
  char blue;
};

class PixelRing {
  public:
    PixelRing(uint8_t pin, uint16_t numPixels);
    void start(RingState ringState);
    void run(void);
    void changeState(RingState newState);
    void stop(void);

  private:
    uint16_t _numPixels;
    RingState _ringState;
    RingColor _baseColor;
    int _index;
    Adafruit_NeoPixel* _neoPixel;

    void setRingStateAndBaseColor(RingState newRingState);
    void renderBase(void);
    void renderMovement(void);
    int getIndexWithOffset(int offset);
};

#endif
