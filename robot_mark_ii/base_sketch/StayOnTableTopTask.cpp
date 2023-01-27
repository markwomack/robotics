//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// My includes
#include <DebugMsgs.h>
#include <TaskManager.h>

// Local includes
#include "StayOnTableTopTask.h"
#include "EdgeSensors.h"
#include "MotorsAndEncoders.h"
#include "PixelRing.h"

void StayOnTableTopTask::setup(void) {
  _movementState = STOPPED;
  
  // set the stuff into the tasks
  _readEdgeSensorsTask.setEdgeSensors(_edgeSensors);
  _adjustMotorSpeedsTask.setMotorsAndEncoders(_motorsAndEncoders);
  _adjustPixelRingTask.setPixelRing(_pixelRing);
  
  // register the methods used when running this behavior
  taskManager.addTask(&_readEdgeSensorsTask, 50);
  taskManager.addTask(&_adjustMotorSpeedsTask, 10);
  taskManager.addTask(&_adjustPixelRingTask, 50);
}
  
// Defines the behavior of the robot, a set of states that are cycled
// through as sensor data is interpreted.
void StayOnTableTopTask::update(void) {
  switch (_movementState) {
    case STOPPED:
      goForward();
      break;
    case GOFORWARD:
      if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
        if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
          goReverse();
        } else {
          turnForwardRight();
        }
      } else if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
        if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
          goReverse();
        } else {
          turnForwardLeft();
        }
      }
      break;
    case GOREVERSE:
      if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
        if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
          goForward();
        } else {
          turnReverseLeft();
        }
      } else if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
        if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
          goForward();
        } else {
          turnReverseRight();
        }
      }
      break;
    case TURN_F_LEFT:
      if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
        goReverse();
      }
      break;
    case TURN_F_RIGHT:
      if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
        goReverse();
      }
      break;
    case TURN_R_LEFT:
      if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
        goForward();
      }
      break;
    case TURN_R_RIGHT:
      if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
        goForward();
      }
      break;
    default:
      break;
  }
}
  
void StayOnTableTopTask::stop(void) {
  // Stop the robot
  _motorsAndEncoders->stop();
  _movementState = STOPPED;
}
