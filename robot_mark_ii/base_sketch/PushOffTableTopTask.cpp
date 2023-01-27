//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// My includes
#include <DebugMsgs.h>
#include <TaskManager.h>

// Local includes
#include "PushOffTableTopTask.h"

String FieldLocationToStr(FieldLocation fl) {
  switch(fl) {
    case NOT_FOUND:
      return "NOT_FOUND";
    case TO_LEFT:
      return "TO_LEFT";
    case AT_CENTER:
      return "AT_CENTER";
    case TO_RIGHT:
      return "TO_RIGHT";
    case CAPTURED:
      return "CAPTURED";
  }
  return "UNKNOWN";
}
    
const int ACQUISITION_THRESHOLD(50);
const int DISTANCE_THRESHOLD(255);

const int MAX_JIG(2);


void PushOffTableTopTask::setup(void) {

  _distanceSensors-> setDistanceThreshold(DISTANCE_THRESHOLD);
  
  // set the stuff into the tasks
  _readEdgeSensorsTask.setEdgeSensors(_edgeSensors);
  _adjustMotorSpeedsTask.setMotorsAndEncoders(_motorsAndEncoders);
  _adjustPixelRingTask.setPixelRing(_pixelRing);
  
  // register the methods used when running this behavior
  taskManager.addTask(&_readEdgeSensorsTask, 50);
  taskManager.addTask(&_adjustMotorSpeedsTask, 10);
  taskManager.addTask(&_adjustPixelRingTask, 50);
}
  
void PushOffTableTopTask::start(void) {
  _movementState = STOPPED;
  _logicState = SEEKING;
  _jiggerCount = 0;
  _lastFrontLocation = NOT_FOUND;

  // Get an initial read of the edge sensors
  _edgeSensors->read();
}
  
void PushOffTableTopTask::stop() {
  // Stop the robot
  _motorsAndEncoders->stopAndResetEncoders();
  _movementState = STOPPED;
  _logicState = SEEKING;
  _lastFrontLocation = NOT_FOUND;
  _jiggerCount = 0;
}
  
// Implements the behavior of the robot, a set of states that are cycled
// through as sensor data is interpreted.
void PushOffTableTopTask::update(void) {
  if (_movementState != STOPPED && _movementState != TURN_F_LEFT && _movementState != TURN_F_RIGHT &&
      _movementState != TURN_R_LEFT && _movementState != TURN_R_RIGHT && _logicState != PUSHING_OFF) {
    if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
      if (_movementState != GOREVERSE) {
        DebugMsgs.println("detected edge: front left");
        if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
          DebugMsgs.println("front right edge too!");
          if (_logicState == ACQUIRED) {
            stopMovement();
          } else {
            goReverse(150);
          }
        } else {
          turnForwardRight();
        }
        return;
      }
    } else if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
      if (_movementState != GOREVERSE) {
        DebugMsgs.println("detected edge: front right");
        if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
          DebugMsgs.println("front left edge too!");
          if (_logicState == ACQUIRED) {
            stopMovement();
          } else {
            goReverse(150);
          }
        } else {
          turnForwardLeft();
        }
        return;
      }
    } else if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
      if (_movementState != GOFORWARD) {
        DebugMsgs.println("detected edge: rear left");
        if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
          DebugMsgs.println("rear right edge too!");
          if (_logicState == ACQUIRED) {
            stopMovement();
          } else {
            goForward(150);
          }
        } else {
          turnReverseLeft();
        }
        return;
      }
    } else if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
      if (_movementState != GOFORWARD) {
        DebugMsgs.println("detected edge: rear right");
        if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
          DebugMsgs.println("rear left edge too!");
          if (_logicState == ACQUIRED) {
            stopMovement();
          } else {
            goForward(150);
          }
        } else {
          turnReverseRight();
        }
        return;
      }
    }
  }

  // These are movements we treat the same for all logic states
  switch(_movementState) {
    case TURN_F_LEFT: {
      if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD) {
        if (_logicState == ACQUIRED || _logicState == PUSHING_OFF) {
          stopMovement();
        } else {
          goReverse(150);
        }
      }
      return;
    }
    break;
    
    case TURN_F_RIGHT: {
      if (_edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
        if (_logicState == ACQUIRED || _logicState == PUSHING_OFF) {
          stopMovement();
        } else {
          goReverse(150);
        }
      }
      return;
    }
    break;
    
    case TURN_R_LEFT: {
      if (_edgeSensors->value(RR_EDGE) > EDGE_THRESHOLD) {
        if (_logicState == ACQUIRED || _logicState == PUSHING_OFF) {
          stopMovement();
        } else {
          goForward(150);
        }
      }
      return;
    }
    break;
    
    case TURN_R_RIGHT: {
      if (_edgeSensors->value(RL_EDGE) > EDGE_THRESHOLD) {
        if (_logicState == ACQUIRED || _logicState == PUSHING_OFF) {
          stopMovement();
        } else {
          goForward(150);
        }
      }
    }
    break;
    
    case GOFORWARD: {
      if (_targetTicks != 0 && _motorsAndEncoders->readEncoder(RIGHT_MOTOR) >= _targetTicks) {
        stopMovement();
      }
      if (_logicState != LOCATED) {
        return;
      }
    }
    break;

    case GOREVERSE: {
      if (_targetTicks != 0 && _motorsAndEncoders->readEncoder(RIGHT_MOTOR) <= _targetTicks) {
        stopMovement();
      }
      return;
    }
    break;
    
    default:
      // fallthru to code below
    break;
  }

  switch(_logicState) {
    case SEEKING:
      handleSeekingState();
      break;

    case SPIN_SEEKING:
      handleSpinSeekingState();
      break;
      
    case LOCATED:
      handleLocatedState();
      break;

    case ACQUIRED:
      handleAcquiredState();
      break;

    case PUSHING_OFF:
      handlePushingOff();
      break;

    case CELEBRATE:
      handleCelebrate();
      break;

    case DONE:
      DebugMsgs.println("DONE!");
      stop();
      stopMovement();
      taskManager.stop();
      break;
  }
}

void PushOffTableTopTask::handleSeekingState(void) {
  
  switch(_movementState) {
    case STOPPED: {
      // Check the front sensor
      DebugMsgs.println("object not sighted, seeking at front");
      FieldLocation objectLocation = checkForObject(FRONT);
      DebugMsgs.print("Front sensors before adjustment: ").println(FieldLocationToStr(objectLocation));
      objectLocation = adjustForJigger(objectLocation);
      DebugMsgs.print("Front sensors: ").println(FieldLocationToStr(objectLocation));
      switch(objectLocation) {
        case TO_LEFT:
          spin(-2);
          break;
        case TO_RIGHT:
          spin(2);
          break;
        case AT_CENTER:
          _logicState = LOCATED;
          break;
        case CAPTURED:
          _logicState = ACQUIRED;
          break;
        case NOT_FOUND:
          // do nothing, call thru to check rear sensors
          break;
      }
      _lastFrontLocation = objectLocation;
      
      // If the logic or movement states have changed, return
      if (_logicState != SEEKING || _movementState != STOPPED) {
        return;
      }

      _jiggerCount = 0;
      
      // check the rear sensor
      DebugMsgs.println("object not sighted, seeking at rear");
      objectLocation = checkForObject(REAR);
      DebugMsgs.print("Rear sensors: ").println(FieldLocationToStr(objectLocation));
      switch(objectLocation) {
        case NOT_FOUND:
          if (_movementState != SPIN_LEFT && _movementState != SPIN_RIGHT) {
            _logicState = SPIN_SEEKING;
          }
          break;
        case TO_LEFT:
          // turn to the right to center on the object
          spin(170);
          break;
        case TO_RIGHT:
          // turn to the left to center on the object
          spin(-170);
          break;
        case AT_CENTER:
        case CAPTURED: // the object can't be captured at the rear, but the action is the same
          // turn almost 180 to center on the object
          spin(175);
          break;
      }
    }
    break;

    case SPIN_LEFT:
    case SPIN_RIGHT: {
      unsigned int currentTicks = _motorsAndEncoders->readEncoder(_movementState == SPIN_LEFT ? RIGHT_MOTOR : LEFT_MOTOR);
      if (currentTicks >= _targetTicks) {
        stopMovement();
      }
    }
    break;
      
    default:
      break;
  }
}

void PushOffTableTopTask::handleSpinSeekingState(void) {
  switch(_movementState) {
    case STOPPED: {
      spin(-270);
    }
    break;

    case SPIN_LEFT: {
      if (_distanceSensors->getDistanceFromObject(FRONT, LEFT) < DISTANCE_THRESHOLD) {
        DebugMsgs.println("Spin seeking front left found object, switching to seeking");
        stopMovement();
        _logicState = SEEKING;
      } else if (_distanceSensors->getDistanceFromObject(REAR, LEFT) < DISTANCE_THRESHOLD) {
        DebugMsgs.println("Spin seeking rear left found object, switching to seeking");
        stopMovement();
        _logicState = SEEKING;
        // turn to the right to center on the object
        DebugMsgs.println("Spinning right to center on object");
        spin(170);
      } else {
        unsigned int currentTicks = _motorsAndEncoders->readEncoder(RIGHT_MOTOR);
        if (currentTicks >= _targetTicks) {
          DebugMsgs.println("Spin seeking did not find object, switch back to seeking and go forward");
          _logicState = SEEKING;
          goForward(400);
        }
      }
    }
    break;
      
    case SPIN_RIGHT: {
      if (_distanceSensors->getDistanceFromObject(FRONT, RIGHT) < DISTANCE_THRESHOLD) {
        DebugMsgs.println("Spin seeking front right found object, switching to seeking");
        stopMovement();
        _logicState = SEEKING;
      } else if (_distanceSensors->getDistanceFromObject(REAR, RIGHT) < DISTANCE_THRESHOLD) {
        DebugMsgs.println("Spin seeking rear right found object, switching to seeking");
        stopMovement();
        _logicState = SEEKING;
        // turn to the left to center on the object
        DebugMsgs.println("Spinning left to center on object");
        spin(-170);
      } else {
        unsigned int currentTicks = _motorsAndEncoders->readEncoder(LEFT_MOTOR);
        if (currentTicks >= _targetTicks) {
          DebugMsgs.println("Spin seeking did not find object, switch back to seeking and go forward");
          _logicState = SEEKING;
          goForward(150);
        }
      }
    }
    break;

    default:
    break;
  }
}

void PushOffTableTopTask::handleLocatedState(void) {
  
  switch(_movementState) {
    case STOPPED: {
      int distance = _distanceSensors->getDistanceFromObject(FRONT, CENTER);
      DebugMsgs.print("Distance to object: ").println(distance);
      if (distance >= DISTANCE_THRESHOLD) {
        DebugMsgs.println("Lost object, switching to seeking");
        _logicState = SEEKING;
        return;
      } else if (distance <= ACQUISITION_THRESHOLD) {
        _logicState = ACQUIRED;
        return;
      }
      int mmDistanceToAcquire = max(10, distance - ACQUISITION_THRESHOLD);
      DebugMsgs.print("Moving forward to acquire objectm acquisitionDistance: ").println(mmDistanceToAcquire);
      goForward(mmDistanceToAcquire);
    }
    break;

    default:
    break;
  }
}

void PushOffTableTopTask::handleAcquiredState() {
  // If acqusition has been lost, then back up and go back to seeking
  if (_distanceSensors->getDistanceFromObject(FRONT, CENTER) >= ACQUISITION_THRESHOLD) {
    DebugMsgs.println("object pushed off the table");
    _logicState = CELEBRATE;
    goReverse(100);
    return;
  }

  if (_movementState == STOPPED) {
    if (_edgeSensors->value(FL_EDGE) > EDGE_THRESHOLD || 
          _edgeSensors->value(FR_EDGE) > EDGE_THRESHOLD) {
      DebugMsgs.println("At the edge, switching to pushing off");
      _logicState = PUSHING_OFF;
    } else {
      DebugMsgs.println("Moving foward to push off the object");
      goForward();
    }
  }
}

void PushOffTableTopTask::handlePushingOff() {
  // If acqusition has been lost, then the object fell off the tabel
  if (_distanceSensors->getDistanceFromObject(FRONT, CENTER) >= ACQUISITION_THRESHOLD) {
    DebugMsgs.println("object pushed off the table");
    _logicState = CELEBRATE;
    goReverse(100);
    return;
  }
  
  if (_movementState == STOPPED) {
    DebugMsgs.println("Creeping forward");
    goForward(2);
  }
}

void PushOffTableTopTask::handleCelebrate() {
  switch(_movementState) {
    case STOPPED: {
      spinFast(1080);
    }
    break;
    
    case SPIN_RIGHT: {
      if (_motorsAndEncoders->readEncoder(LEFT_MOTOR) >= _targetTicks) {
        stopMovement();
        _logicState = DONE;
      }
    }
    break;

    default:
    break;
  }
}
  
FieldLocation PushOffTableTopTask::adjustForJigger(FieldLocation inputFieldLocation) {
  DebugMsgs.print("_jiggerCount in = ").println(_jiggerCount);
  if (inputFieldLocation == TO_LEFT || inputFieldLocation == TO_RIGHT) {
    if ((_jiggerCount / 2) >= MAX_JIG) {
      DebugMsgs.print("The jig is up, setting AT_CENTER");
      _jiggerCount = 0;
      return AT_CENTER;
    }
  }

  if (inputFieldLocation == TO_LEFT) {
    if (_lastFrontLocation == TO_RIGHT) {
      _jiggerCount++;
    } else {
      _jiggerCount = 0;
    }
  } else if (inputFieldLocation == TO_RIGHT) {
    if (_lastFrontLocation == TO_LEFT) {
      _jiggerCount++;
    } else {
      _jiggerCount = 0;
    }
  } else {
    _jiggerCount = 0;
  }
  
  DebugMsgs.print("_jiggerCount out = ").println(_jiggerCount);
  return inputFieldLocation;
}
  
FieldLocation PushOffTableTopTask::checkForObject(Orientation orientation) {

  unsigned int left = _distanceSensors->getDistanceFromObject(orientation, LEFT);
  unsigned int center = _distanceSensors->getDistanceFromObject(orientation, CENTER);
  unsigned int right = _distanceSensors->getDistanceFromObject(orientation, RIGHT);
  DebugMsgs.print("left: ").print(left);
  DebugMsgs.print(" center: ").print(center);
  DebugMsgs.print(" right: ").println(right);

  if (left >= DISTANCE_THRESHOLD && center >= DISTANCE_THRESHOLD && right >= DISTANCE_THRESHOLD) {
    return NOT_FOUND;
  } else if (center <= ACQUISITION_THRESHOLD) {
    return CAPTURED;
  } else if (center < left && center < right) {
    return AT_CENTER;
  } else if (right < center) {
    return TO_RIGHT;
  } else if (left < center) {
    return TO_LEFT;
  }
  return NOT_FOUND;
}
