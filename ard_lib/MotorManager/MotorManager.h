//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef MotorManager_h
#define MotorManager_h

// Arduino includes
#include <inttypes.h>

// Local includes
#include "MotorEncoder.h"

enum Motor {
  LEFT_MOTOR,
  RIGHT_MOTOR
};

/*
 * Defines the interface for motor management.
 */
class MotorManager
{

public:
  MotorManager(){};

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
   * Assigns the motor encoders to use.
   */
  void setupEncoders(
    MotorEncoder* leftMotorEncoder, MotorEncoder* rightMotorEncoder);

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
   * Reads the value of the given motors encoder faults.
   */
  int32_t readEncoderFaults(Motor motor);

  /*
   * Sets the value of the given motors encoder to the given value.
   */
  void writeEncoder(Motor motor, int32_t value);

protected:
  MotorEncoder* _leftEncoder;
  MotorEncoder* _rightEncoder;
};

#endif
