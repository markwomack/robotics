/*
 *      Please see the README.txt for a full description of this program and
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
