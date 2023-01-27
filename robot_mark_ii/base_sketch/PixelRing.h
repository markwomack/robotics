//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PIXELRING_H
#define PIXELRING_H

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
    PixelRing() {};

    void initialize(void);
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

#endif // PIXELRING_H
