//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//


// Local includes
#include "QuadratureMotorEncoder.h"

QuadratureMotorEncoder::QuadratureMotorEncoder(int8_t phaseAPin, int8_t phaseBPin) {
  _encoder = new Encoder(phaseAPin, phaseBPin);
}

int32_t QuadratureMotorEncoder::read(void) {
  return _encoder->read();
}

int32_t QuadratureMotorEncoder::write(int32_t value) {
  int32_t curValue = _encoder->read();
  _encoder->write(value);
  return curValue;
}

int32_t QuadratureMotorEncoder::readFaults(void) {
  return 0; // Fault reporting is not supported
}