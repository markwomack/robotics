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

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

// Third party includes
#include <PID_v1.h>

// My includes
#include <MotorManager.h>

class MotorController {
  public:
    MotorController(MotorManager* motorManager, double Kp, double Ki, double Kd, unsigned int frequency,
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
    
    MotorManager* _motorManager;
};

#endif
