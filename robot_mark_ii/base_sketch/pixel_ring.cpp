//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Local includes
#include "pixel_ring.h"

PixelRing::PixelRing(uint8_t pin, uint16_t numPixels) {
  _numPixels = numPixels;
  _ringState = RING_OFF;
  _neoPixel = new Adafruit_NeoPixel(_numPixels, pin, NEO_GRBW + NEO_KHZ800);
  _neoPixel->begin();
  _neoPixel->clear();
  _neoPixel->show();
}

void PixelRing::start(RingState ringState) {
  setRingStateAndBaseColor(ringState);
  _index = 0;
  
  switch(_ringState) {
    case RING_OFF:
      _neoPixel->clear();
      _neoPixel->show();
      break;
    default:
      renderBase();
      break;
  }
}

void PixelRing::run(void) {
  if (_ringState == RING_WHITE_FADE) {
    if (_index == -1 && _baseColor.red == 2) {
      _index = 1;
    } else if (_index == 1 && _baseColor.red == 30) {
      _index = -1;
    }
    _baseColor.red += _index;
    _baseColor.green += _index;
    _baseColor.blue += _index;
    renderMovement();
  } else {
    renderMovement();
    _index = getIndexWithOffset(1);
  }
}

void PixelRing::changeState(RingState newState) {
  if (_ringState == newState) {
    return;
  }
  
  setRingStateAndBaseColor(newState);

  switch(_ringState) {
    case RING_OFF:
      _neoPixel->clear();
      _neoPixel->show();
      break;
    default:
      renderBase();
      renderMovement();
      break;
  }
}

void PixelRing::stop(void) {
  _ringState = RING_OFF;
  _neoPixel->clear();
  _neoPixel->show();
}

void PixelRing::setRingStateAndBaseColor(RingState newRingState) {
  _ringState = newRingState;

  switch(_ringState) {
    case RING_GREEN_CW:
      _baseColor.red = 0;
      _baseColor.green = 5;
      _baseColor.blue = 0;
      break;
    case RING_RED_CW:
      _baseColor.red = 5;
      _baseColor.green = 0;
      _baseColor.blue = 0;
      break;
    case RING_BLUE_CW:
      _baseColor.red = 0;
      _baseColor.green = 0;
      _baseColor.blue = 5;
      break;
    case RING_WHITE_FADE:
      _baseColor.red = 2;
      _baseColor.green = 2;
      _baseColor.blue = 2;
      _index = 1;
    default:
      _baseColor.red = 0;
      _baseColor.green = 0;
      _baseColor.blue = 0;
      break;
  }
}
void PixelRing::renderBase(void){
  for (int i = 0; i < _numPixels; i++) {
    _neoPixel->setPixelColor(i, _neoPixel->Color(_baseColor.red, _baseColor.green, _baseColor.blue));
  }
  _neoPixel->show();
}

void PixelRing::renderMovement(void) {
  if (_ringState == RING_WHITE_FADE) {
    for (int i = 0; i < _numPixels; i++) {
      _neoPixel->setPixelColor(i, _neoPixel->Color(_baseColor.red, _baseColor.green, _baseColor.blue));
    }
  } else {
    for (int offset = -1; offset < 3; offset++) {
      int index = getIndexWithOffset(offset);
      uint32_t color;
      
      switch(offset) {
        case 0:
          color = _neoPixel->Color((_baseColor.red != 0) ? _baseColor.red + (255 -_baseColor.red)/4 : 0,
                                   (_baseColor.green != 0) ? _baseColor.green + (255 -_baseColor.green)/4 : 0,
                                   (_baseColor.blue != 0) ? _baseColor.blue + (255 -_baseColor.blue)/4 : 0);
          break;
        case 1:
          color = _neoPixel->Color((_baseColor.red != 0) ? _baseColor.red + (255 -_baseColor.red)/2 : 0,
                                   (_baseColor.green != 0) ? _baseColor.green + (255 -_baseColor.green)/2 : 0,
                                   (_baseColor.blue != 0) ? _baseColor.blue + (255 -_baseColor.blue)/2 : 0);
          break;
        case 2:
          color = _neoPixel->Color((_baseColor.red != 0) ? 255 : 0,
                                   (_baseColor.green != 0) ? 255: 0,
                                   (_baseColor.blue != 0) ? 255 : 0);
          break;
        default:
          color = _neoPixel->Color(_baseColor.red, _baseColor.green, _baseColor.blue);
          break;
      }
      _neoPixel->setPixelColor(index, color);
    }
  }
  _neoPixel->show();
}

int PixelRing::getIndexWithOffset(int offset) {
  int newValue;
  if (_index == 0 && offset == -1) {
    newValue = 15;
  } else
    newValue = (_index + offset) % _numPixels;
  return newValue;
}
