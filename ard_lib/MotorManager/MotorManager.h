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

#ifndef MotorManager_h
#define MotorManager_h

#include <inttypes.h>
#include <Encoder.h>

enum Motor {
  LEFT_MOTOR,
  RIGHT_MOTOR
};

class MotorManager
{

public:
  /*
   * Sets the speed of the given motor. Speed is expected to be a 
   * value between -1 and 1.
   */

  virtual void setMotorSpeed(Motor motor, double speed) = 0;

  /*
   * Sets the speed of the motors. Speeds are expected to be a 
   * value between -1 and 1.
   */

  virtual void setMotorSpeeds(double speedLeft, double speedRight) = 0;
  /*
   * Assigns the pins that have the quadrature phase signals.
   */

  void setupEncoders(
    uint8_t leftEncoderPhaseAPin, uint8_t leftEncoderPhaseBPin,
    uint8_t rightEncoderPhaseAPin, uint8_t rightEncoderPhaseBPin);
  /*
   * Reads the value of the given motors encoder.
   */

  int32_t readEncoder(Motor motor);
  /*
   * Reads the value of the given motors encoder and then resets the
   * value to zero.
   */

  int32_t readAndResetEncoder(Motor motor);

  /*
   * Sets the value of the given motors encoder to the given value.
   */
  void writeEncoder(Motor motor, int32_t value);

protected:
  Encoder* _leftEncoder;
  Encoder* _rightEncoder;
};

#endif
