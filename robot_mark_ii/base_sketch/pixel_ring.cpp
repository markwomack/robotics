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
  renderMovement();
  _index = getIndexWithOffset(1);
}

void PixelRing::changeState(RingState newState) {
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
