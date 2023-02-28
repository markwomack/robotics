//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef ANIMATION_H
#define ANIMATION_H

// Third party includes
#include <Adafruit_NeoPixel.h>
#include <PixelRingAnimator.h>

// Local includes
#include "pin_assignments.h"
#include "robot_constants.h"

enum AnimationState {
  OFF,
  GREEN_CW,
  RED_CW,
  BLUE_CW,
  GREEN_COUNTDOWN,
  WHITE_FADE,
  BLUE_FADE
};

class Animation {
  public:
    Animation() {};

    void initialize(void) {
      _pixelRingAnimator.start(PIXEL_RING_PIN, NUM_PIXELS_ON_RING, NEOPIXEL_TYPE, START_PIXEL_INDEX);
    };

    void setAnimationState(AnimationState animationState) {
      switch(animationState) {
        case OFF: {
          _pixelRingAnimator.doOff();
        }
        break;

        case GREEN_CW: {
          PixelRingColor green(0, 5, 0);
          _pixelRingAnimator.doSpin(&green, false);
        }
        break;

        case RED_CW: {
          PixelRingColor red(5, 0, 0);
          _pixelRingAnimator.doSpin(&red, false);
        }
        break;

        case BLUE_CW: {
          PixelRingColor blue(0, 0, 5);
          _pixelRingAnimator.doSpin(&blue, false);
        }
        break;

        case GREEN_COUNTDOWN: {
          PixelRingColor baseColor(0, 0, 0);
          PixelRingColor green(0, 5, 0);
          _pixelRingAnimator.doCountdown(&baseColor, &green, false);
        }
        break;

        case WHITE_FADE: {
          PixelRingColor start(2, 2, 2);
          PixelRingColor end(30, 30, 30);
          _pixelRingAnimator.doFade(&start, &end, 28);
        }
        break;

        case BLUE_FADE: {
          PixelRingColor start(0, 0, 2);
          PixelRingColor end(0, 0, 30);
          _pixelRingAnimator.doFade(&start, &end, 28);
        }
        break;
      }
    };

    void update(void) {
      _pixelRingAnimator.update();
    }

  private:
    PixelRingAnimator _pixelRingAnimator;
  
};
#endif // ANIMATION_H
