//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

// Third party includes
#include <PID_v1.h>

// My includes
#include <MotorAndEncoderManager.h>

class MotorController {
  public:
    MotorController(MotorAndEncoderManager* motorManager, double Kp, double Ki, double Kd, unsigned int frequency,
      double radiansPerTick, double maxRadiansPerSecond);
    void start();
    bool isRunning();
    void setDesiredSpeeds(double leftMotorSpeed, double rightMotorSpeed);
    bool adjustSpeeds();
    void stop();

  private:
    bool _isRunning;
    
    double _Kp;
    double _Ki;
    double _Kd;

    double _radiansPerTick;
    double _maxRadiansPerSecond;

    double _leftMotorSetpoint;
    double _leftMotorInput;
    double _leftMotorOutput;
    double _leftLastSpeed;
    long _last_encoder_left;
    PID* _leftMotorPID;

    double _rightMotorSetpoint;
    double _rightMotorInput;
    double _rightMotorOutput;
    double _rightLastSpeed;
    long _last_encoder_right;
    PID* _rightMotorPID;
    
    unsigned long _last_encoder_read_time;
    
    MotorAndEncoderManager* _motorManager;
};

#endif
