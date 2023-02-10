//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include "MotorManager.h"

void MotorManager::setupEncoders(MotorEncoder* leftMotorEncoder, MotorEncoder* rightMotorEncoder) {
  _leftEncoder = leftMotorEncoder;
  _rightEncoder = rightMotorEncoder;
}

int32_t MotorManager::readEncoder(Motor motor) {
  return motor == LEFT_MOTOR ? _leftEncoder->read() : _rightEncoder->read();
}

int32_t MotorManager::readAndResetEncoder(Motor motor) {
  return motor == LEFT_MOTOR ? _leftEncoder->write(0) : _rightEncoder->write(0);
}

int32_t MotorManager::readEncoderFaults(Motor motor) {
  return motor == LEFT_MOTOR ? _leftEncoder->readFaults() : _rightEncoder->readFaults();
}

void MotorManager::writeEncoder(Motor motor, int32_t value) {
  if (motor == LEFT_MOTOR) {
    _leftEncoder->write(value);
  } else {
    _rightEncoder->write(value);
  }
}
