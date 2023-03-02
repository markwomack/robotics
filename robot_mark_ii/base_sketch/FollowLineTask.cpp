//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// My includes
#include <TaskManager.h>

// Local includes
#include "FollowLineTask.h"
#include "robot_constants.h"
#include "SurfaceSensors.h"

const uint16_t LINE_THRESHOLD(150);

void FollowLineTask::setup(void) {
  _movementState = STOPPED;
  _motorsAndEncoders->stopAndResetEncoders();
  
  // set the sensors into the tasks
  _readSurfaceSensorsTask.setSurfaceSensors(_surfaceSensors);
  _adjustMotorSpeedsTask.setMotorsAndEncoders(_motorsAndEncoders);
  _adjustAnimationTask.setAnimation(_animation);

  // register the tasks used when running this behavior
  taskManager.addTask(&_readSurfaceSensorsTask, 50);
  taskManager.addTask(&_adjustMotorSpeedsTask, 10);
  taskManager.addTask(&_adjustAnimationTask, 50);
}

void FollowLineTask::start(void) {
  _currentState = AT_START;
  _lineLocation = LINE_IS_LOST;
}

LineLocation FollowLineTask::checkLineLocation(void) {
  if (_centerValues[0] >= 150 || _centerValues[1] >= 150) {
    return LINE_IS_LEFT;
  } else if (_centerValues[2] >= 150 || _centerValues[3] >= 150) {
    return LINE_IS_CENTER;
  } else if (_centerValues[4] >= 150 || _centerValues[5] >= 150) {
    return LINE_IS_RIGHT;
  } else {
    return LINE_IS_LOST;
  }
}

void FollowLineTask::update(void) {

  // read the current sensor values
  _surfaceSensors->getSensorValues(S_FRONT, _frontValues);
  _surfaceSensors->getSensorValues(S_CENTER, _centerValues);
  _surfaceSensors->getSensorValues(S_REAR, _rearValues);

  switch(_currentState) {
    case AT_START: {
      _currentState = FOLLOW;
      goForward(FOLLOW_SPEED, FOLLOW_SPEED);
    }
    break;

    case FOLLOW: {
      LineLocation newLineLocation = checkLineLocation();
      if (newLineLocation == LINE_IS_LOST) {
        _currentState = AT_STOP;
      }
      if (newLineLocation == _lineLocation) {
        break;
      }
      _lineLocation = newLineLocation;
      switch(_lineLocation) {
        case LINE_IS_LEFT: {
          goForward(FOLLOW_SPEED - 0.1, FOLLOW_SPEED + 0.1);
        }
        break;
        
        case LINE_IS_CENTER: {
          goForward(FOLLOW_SPEED, FOLLOW_SPEED);
        }
        break;
        
        case LINE_IS_RIGHT: {
          goForward(FOLLOW_SPEED + 0.1, FOLLOW_SPEED - 0.1);
        }
        break;
        
        case LINE_IS_LOST: {
          _currentState = AT_STOP;
        }
      }
    }
    break;

    case AT_STOP: {
      stopMovement();
      taskManager.stop();
    }
    break;
  }

  DebugMsgs.debug().print("Current state: ").println(_currentState);
  DebugMsgs.debug().print("Current location: ").println(_lineLocation);
  
  if (DebugMsgs.isLevelEnabled(DEBUG)) {
    for (int x = 0; x < NUM_FRONT_SURFACE_SENSORS; x++) {
      DebugMsgs.print(_frontValues[x]).print(" ");
    }
    DebugMsgs.println();
    for (int x = 0; x < NUM_CENTER_SURFACE_SENSORS; x++) {
      DebugMsgs.print(_centerValues[x]).print(" ");
    }
    DebugMsgs.println();
    for (int x = 0; x < NUM_REAR_SURFACE_SENSORS; x++) {
      DebugMsgs.print(_rearValues[x]).print(" ");
    }
    DebugMsgs.println();
  }
}

void FollowLineTask::stop(void) {
  stopMovement();
}
