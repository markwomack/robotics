//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

// Local includes
#include "MotorController.h"

MotorController::MotorController(MotorAndEncoderManager* motorManager, double Kp, double Ki, double Kd,
    unsigned int frequency, double radiansPerTick, double maxRadiansPerSecond) {
  _motorManager = motorManager;
  
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;

  _radiansPerTick = radiansPerTick;
  _maxRadiansPerSecond = maxRadiansPerSecond;
  
  _leftMotorSetpoint = 0.0;
  _leftMotorInput = 0.0;
  _leftMotorOutput = 0.0;
  _leftLastSpeed = 0.0;

  _rightMotorSetpoint = 0.0;
  _rightMotorInput = 0.0;
  _rightMotorOutput = 0.0;
  _rightLastSpeed = 0.0;

  _last_encoder_left = 0;
  _last_encoder_right = 0;
  _last_encoder_read_time = 0;

  _isRunning = false;

  _leftMotorPID = new PID(&_leftMotorInput, &_leftMotorOutput, &_leftMotorSetpoint, _Kp, _Ki, _Kd, DIRECT);
  _leftMotorPID->SetSampleTime(frequency);
  _leftMotorPID->SetOutputLimits(-maxRadiansPerSecond, maxRadiansPerSecond);
  _rightMotorPID = new PID(&_rightMotorInput, &_rightMotorOutput, &_rightMotorSetpoint, _Kp, _Ki, _Kd, DIRECT);
  _rightMotorPID->SetSampleTime(frequency);
  _rightMotorPID->SetOutputLimits(-maxRadiansPerSecond, maxRadiansPerSecond);
}

void MotorController::start() {
  _motorManager->readAndResetEncoder(M0);
  _motorManager->readAndResetEncoder(M1);
  
  _last_encoder_left = 0;
  _last_encoder_right = 0;
  _last_encoder_read_time = millis();
  
  _leftMotorSetpoint = 0.0;
  _leftMotorInput = 0.0;
  _leftMotorOutput = 0.0;
  _leftLastSpeed = 0.0;

  _rightMotorSetpoint = 0.0;
  _rightMotorInput = 0.0;
  _rightMotorOutput = 0.0;
  _rightLastSpeed = 0.0;
  
  _leftMotorPID->SetMode(AUTOMATIC);
  _rightMotorPID->SetMode(AUTOMATIC);

  _isRunning = true;
}

bool MotorController::isRunning() {
  return _isRunning;
}

void MotorController::setDesiredSpeeds(double leftMotorSpeed, double rightMotorSpeed) {
  _leftMotorSetpoint = leftMotorSpeed;
  _rightMotorSetpoint = rightMotorSpeed;
}

bool MotorController::adjustSpeeds() {

  if (!_isRunning) {
    return false;
  }
  
  // Read the current time and encoder values
  unsigned long current_time = millis();
  long encoder_left = _motorManager->readEncoder(M0);
  long encoder_right = _motorManager->readEncoder(M1);

  // Calculate the distance traveled since last call, in radians
  double diff_l = static_cast<double>(encoder_left - _last_encoder_left) * _radiansPerTick;
  double diff_r = static_cast<double>(encoder_right - _last_encoder_right) * _radiansPerTick;
  double diff_t = (current_time - _last_encoder_read_time)/1000.0;

  // Calculate the current speed in radians/second
  _leftMotorInput = (diff_l/diff_t);
  _rightMotorInput = (diff_r/diff_t);

  // Run the new values through the pids
  bool changeMotorSpeeds = false;
  if (_leftMotorPID->Compute()) {
    _leftLastSpeed = min(max(_leftLastSpeed + _leftMotorOutput, -_maxRadiansPerSecond), _maxRadiansPerSecond);
    changeMotorSpeeds = true;
  }
  if (_rightMotorPID->Compute()) {
    _rightLastSpeed = min(max(_rightLastSpeed + _rightMotorOutput, -_maxRadiansPerSecond), _maxRadiansPerSecond);
    changeMotorSpeeds = true;
  }

  // If the pids adjusted the motor speeds, apply the new speeds to the motor
  if (changeMotorSpeeds) {
    _motorManager->setMotorSpeeds((float)(_leftLastSpeed/_maxRadiansPerSecond), (float)(_rightLastSpeed/_maxRadiansPerSecond));
  }

  // Remember time and encoder values for next time
  _last_encoder_left = encoder_left;
  _last_encoder_right = encoder_right;
  _last_encoder_read_time = current_time;

  return changeMotorSpeeds;
}

void MotorController::stop() {
  _motorManager->setMotorSpeeds(0, 0);
  _isRunning = false;
  _leftMotorPID->SetMode(MANUAL);
  _rightMotorPID->SetMode(MANUAL);
}
