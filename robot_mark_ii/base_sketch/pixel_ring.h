/*
 *      Please see the README.md for a full description of this program and
 *      project.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

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
