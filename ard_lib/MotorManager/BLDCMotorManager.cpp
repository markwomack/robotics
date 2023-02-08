//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include "BLDCMotorManager.h"

BLDCMotorManager::BLDCMotorManager(uint8_t m1SpeedPin, uint8_t m1DirPin, uint8_t m1BrakePin,
   uint8_t m2SpeedPin, uint8_t m2DirPin, uint8_t m2BrakePin) {
  
  _m1SpeedPin = m1SpeedPin;
  _m1DirPin = m1DirPin;
  _m1BrakePin = m1BrakePin;
  _m2SpeedPin = m2SpeedPin;
  _m2DirPin = m2DirPin;
  _m2BrakePin = m2BrakePin;

  // Setup the speed pins
  pinMode(_m1SpeedPin, OUTPUT);
  pinMode(_m2SpeedPin, OUTPUT);
  
  // TODO: put these under some kind of IFDEF
//  // Teensy 4.1 TODO: are these valid for Teensy 4.0?
//  // values will be 0-8191
  analogWriteResolution(13);
  analogWriteFrequency(_m1SpeedPin, 18310.55);
  analogWriteFrequency(_m2SpeedPin, 18310.55);
  
  analogWrite(_m1SpeedPin, 0);
  analogWrite(_m2SpeedPin, 0);

  // Setup brake pins, engage brake
  pinMode(_m1BrakePin, OUTPUT);
  digitalWrite(_m1BrakePin, HIGH);
  pinMode(_m2BrakePin, OUTPUT);
  digitalWrite(_m2BrakePin, HIGH);

  // Setup direction pins, set for forward
  pinMode(_m1DirPin, OUTPUT);
  digitalWrite(_m1DirPin, LOW);
  pinMode(_m2DirPin, OUTPUT);
  digitalWrite(_m2DirPin, LOW);
}

int BLDCMotorManager::mapSpeed(double input) {
  return (int)(min(max(input, -1.0), 1.0) * 8191);
}

void BLDCMotorManager::setMotorSpeed(Motor motor, double speed) {
  if (motor == LEFT_MOTOR) {
    analogWrite(_m1SpeedPin, abs(mapSpeed(speed)));
    digitalWrite(_m1DirPin, speed >= 0 ? LOW : HIGH);
  } else {
    analogWrite(_m2SpeedPin, abs(mapSpeed(speed)));
    digitalWrite(_m2DirPin, speed >= 0 ? LOW : HIGH);
  }
}

void BLDCMotorManager::setMotorSpeeds(double speedLeft, double speedRight) {
  analogWrite(_m1SpeedPin, abs(mapSpeed(speedLeft)));
  digitalWrite(_m1DirPin, speedLeft >= 0 ? LOW : HIGH);
  analogWrite(_m2SpeedPin, abs(mapSpeed(speedRight)));
  digitalWrite(_m2DirPin, speedRight >= 0 ? LOW : HIGH);
}
