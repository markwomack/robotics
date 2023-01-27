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

void BehaviorTask::setEdgeSensors(EdgeSensors* edgeSensors) {
  _edgeSensors = edgeSensors;
}

void BehaviorTask::setDistanceSensors(DistanceSensors* distanceSensors) {
  _distanceSensors = distanceSensors;
}

void BehaviorTask::setMotorsAndEncoders(MotorsAndEncoders* motorsAndEncoders) {
  _motorsAndEncoders = motorsAndEncoders;
}

void BehaviorTask::setPixelRing(PixelRing* pixelRing) {
  _pixelRing = pixelRing;
}

void BehaviorTask::setSurfaceSensors(SurfaceSensors* surfaceSensors) {
  _surfaceSensors = surfaceSensors;
}
    
void BehaviorTask::stopMovement() {
  _movementState = STOPPED;
  _targetTicks = 0;
  _motorsAndEncoders->stopAndResetEncoders();
  _pixelRing->changeState(RING_WHITE_FADE);
  DebugMsgs.println("stopping");
}

void BehaviorTask::spin(int degrees) {
  _motorsAndEncoders->resetEncoders();
  _targetTicks = abs(degrees) * TICKS_PER_BASE_DEGREE;
  if (degrees < 0) {
    _movementState = SPIN_LEFT;
    _motorsAndEncoders->setMotorSpeeds(-SPIN_SPEED, SPIN_SPEED);
    DebugMsgs.print("spinning left ");
  } else if (degrees > 0) {
    _movementState = SPIN_RIGHT;
    _motorsAndEncoders->setMotorSpeeds(SPIN_SPEED, -SPIN_SPEED);
    DebugMsgs.print("spinning right ");
  }
  DebugMsgs.print(degrees).println(" degrees");
  _pixelRing->changeState(RING_BLUE_CW);
}

void BehaviorTask::spinFast(int degrees) {
  _motorsAndEncoders->resetEncoders();
  _targetTicks = abs(degrees) * TICKS_PER_BASE_DEGREE;
  if (degrees < 0) {
    _movementState = SPIN_LEFT;
    _motorsAndEncoders->setMotorSpeeds(-CRUISE_SPEED, CRUISE_SPEED);
    DebugMsgs.print("spinning fast left ");
  } else if (degrees > 0) {
    _movementState = SPIN_RIGHT;
    _motorsAndEncoders->setMotorSpeeds(CRUISE_SPEED, -CRUISE_SPEED);
    DebugMsgs.print("spinning fast right ");
  }
  DebugMsgs.print(degrees).println(" degrees");
  _pixelRing->changeState(RING_BLUE_CW);
}

void BehaviorTask::goForward(unsigned int millimeters) {
  _movementState = GOFORWARD;
  _targetTicks = millimeters * TICKS_PER_MM;
  _motorsAndEncoders->setTargetSpeeds(CRUISE_SPEED, CRUISE_SPEED);
  _pixelRing->changeState(RING_GREEN_CW);
  DebugMsgs.print("going forward");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void BehaviorTask::goReverse(unsigned int millimeters) {
  _movementState = GOREVERSE;
  _targetTicks = millimeters * -TICKS_PER_MM;
  _motorsAndEncoders->setTargetSpeeds(-CRUISE_SPEED, -CRUISE_SPEED);
  _pixelRing->changeState(RING_RED_CW);
  DebugMsgs.print("going reverse");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void BehaviorTask::turnForwardRight() {
  _movementState = TURN_F_RIGHT;
  _motorsAndEncoders->setTargetSpeeds(0, TURN_SPEED);
  _pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn forward right");
}

void BehaviorTask::turnForwardLeft() {
  _movementState = TURN_F_LEFT;
  _motorsAndEncoders->setTargetSpeeds(TURN_SPEED, 0);
  _pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn forward left");
}

void BehaviorTask::turnReverseLeft() {
  _movementState = TURN_R_LEFT;
  _motorsAndEncoders->setTargetSpeeds(0, -TURN_SPEED);
  _pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn reverse left");
}

void BehaviorTask::turnReverseRight() {
  _movementState = TURN_R_RIGHT;
  _motorsAndEncoders->setTargetSpeeds(-TURN_SPEED, 0);
  _pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn reverse right");
}
