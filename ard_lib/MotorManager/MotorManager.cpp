//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include "MotorManager.h"

void MotorManager::setupEncoders(
  uint8_t leftEncoderPhaseAPin, uint8_t leftEncoderPhaseBPin,
  uint8_t rightEncoderPhaseAPin, uint8_t rightEncoderPhaseBPin)
{
  _leftEncoder = new Encoder(leftEncoderPhaseAPin, leftEncoderPhaseBPin);
  _rightEncoder = new Encoder(rightEncoderPhaseAPin, rightEncoderPhaseBPin);
}

int32_t MotorManager::readEncoder(Motor motor)
{
  if (motor == LEFT_MOTOR) {
    return _leftEncoder->read();
  } else {
    return _rightEncoder->read();
  }
}

int32_t MotorManager::readAndResetEncoder(Motor motor)
{
  if (motor == LEFT_MOTOR) {
    return _leftEncoder->readAndReset();
  } else {
    return _rightEncoder->readAndReset();
  }
}

void MotorManager::writeEncoder(Motor motor, int32_t value)
{
  if (motor == LEFT_MOTOR) {
    return _leftEncoder->write(value);
  } else {
    return _rightEncoder->write(value);
  }
}
