//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// My includes
#include <DebugMsgs.h>
#include <TaskManager.h>

// Local includes
#include "BehaviorTask.h"
#include "robot_constants.h"
#include "globals.h"

BehaviorTask::BehaviorTask() {
  // Set members from globals
  _edgeSensors = &edgeSensors;
  _distanceSensors = &distanceSensors;
  _surfaceSensors = &surfaceSensors;
  _motorsAndEncoders = &motorsAndEncoders;
  _animation = &animation;
}

void BehaviorTask::setTaskToken(uint8_t taskToken) {
  _taskToken = taskToken;
}

void BehaviorTask::stopMovement() {
  _movementState = STOPPED;
  _targetTicks = 0;
  _motorsAndEncoders->stopAndResetEncoders();
  _animation->setAnimationState(WHITE_FADE);
  DebugMsgs.println("stopping");
}

void BehaviorTask::spin(double spinSpeed, int degrees) {
  if (_movementState != SPIN_LEFT && _movementState != SPIN_RIGHT) {
    _animation->setAnimationState(BLUE_CW);
  }
  
  _motorsAndEncoders->resetEncoders();
  _targetTicks = abs(degrees) * TICKS_PER_BASE_DEGREE;
  if (degrees < 0) {
    _movementState = SPIN_LEFT;
    _motorsAndEncoders->setMotorSpeeds(-spinSpeed, spinSpeed);
    DebugMsgs.print("spinning left ");
  } else if (degrees > 0) {
    _movementState = SPIN_RIGHT;
    _motorsAndEncoders->setMotorSpeeds(spinSpeed, -spinSpeed);
    DebugMsgs.print("spinning right ");
  }
  DebugMsgs.print(degrees).print(" degrees at speed ").println(spinSpeed);
}

void BehaviorTask::spin(int degrees) {
  spin(SPIN_SPEED, degrees);
}

void BehaviorTask::spinFast(int degrees) {
  spin(CRUISE_SPEED, degrees);
}

void BehaviorTask::goForward(double leftSpeed, double rightSpeed, uint32_t millimeters) {
  if (_movementState != GOFORWARD) {
    _animation->setAnimationState(GREEN_CW);
  }
  _movementState = GOFORWARD;
  _targetTicks = millimeters * TICKS_PER_MM;
  _motorsAndEncoders->setTargetSpeeds(leftSpeed, rightSpeed);
  DebugMsgs.print("going forward");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void BehaviorTask::goForward(uint32_t millimeters) {
  goForward(CRUISE_SPEED, CRUISE_SPEED, millimeters);
}

void BehaviorTask::goReverse(double leftSpeed, double rightSpeed, uint32_t millimeters) {
  if (_movementState != GOREVERSE) {
      _animation->setAnimationState(RED_CW);
  }
  
  _movementState = GOREVERSE;
  _targetTicks = millimeters * -TICKS_PER_MM;
  _motorsAndEncoders->setTargetSpeeds(-CRUISE_SPEED, -CRUISE_SPEED);
  DebugMsgs.print("going reverse");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void BehaviorTask::goReverse(uint32_t millimeters) {
  goReverse(-CRUISE_SPEED, -CRUISE_SPEED, millimeters);
}

void BehaviorTask::turnForwardRight() {
  _movementState = TURN_F_RIGHT;
  _motorsAndEncoders->setTargetSpeeds(0, TURN_SPEED);
  _animation->setAnimationState(BLUE_CW);
  DebugMsgs.println("turn forward right");
}

void BehaviorTask::turnForwardLeft() {
  _movementState = TURN_F_LEFT;
  _motorsAndEncoders->setTargetSpeeds(TURN_SPEED, 0);
  _animation->setAnimationState(BLUE_CW);
  DebugMsgs.println("turn forward left");
}

void BehaviorTask::turnReverseLeft() {
  _movementState = TURN_R_LEFT;
  _motorsAndEncoders->setTargetSpeeds(0, -TURN_SPEED);
  _animation->setAnimationState(BLUE_CW);
  DebugMsgs.println("turn reverse left");
}

void BehaviorTask::turnReverseRight() {
  _movementState = TURN_R_RIGHT;
  _motorsAndEncoders->setTargetSpeeds(-TURN_SPEED, 0);
  _animation->setAnimationState(BLUE_CW);
  DebugMsgs.println("turn reverse right");
}
