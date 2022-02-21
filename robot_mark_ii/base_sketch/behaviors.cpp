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

// Third party includes
#include <PID_v1.h>

// My includes
#include <DebugMsgs.h>
#include <MotorController.h>

// Local includes
#include "behaviors.h"
#include "robot_constants.h"
#include "helper_methods.h"

enum PushObjectOffTableState {
  SEEKING,
  SPIN_SEEKING,
  LOCATED,
  ACQUIRED,
  PUSHING_OFF,
  CELEBRATE,
  DONE
};

enum FieldLocation {
  NOT_FOUND,
  TO_LEFT,
  AT_CENTER,
  TO_RIGHT,
  CAPTURED
};

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

enum SensorLocation {
  SIDE_LEFT = 0,
  LEFT = 1,
  CENTER = 2,
  RIGHT = 3,
  SIDE_RIGHT = 4
};

enum Orientation {
  FRONT,
  REAR
};

// Run the behavior stored in the context pointer.
void runBehavior(void* context) {  
  Behavior* behavior = (Behavior*)context;
  behavior->doBehavior();
}
    
const int ACQUISITION_THRESHOLD(50);
const int DISTANCE_THRESHOLD(255);

const int MAX_JIG(2);

class PushOffTableTopBehavior : public Behavior {
  protected:
    PushObjectOffTableState _logicState;
    ButtonExecutor* _buttonExecutor;
    int _jiggerCount;
    FieldLocation _lastFrontLocation;
    
  public:
  
  PushOffTableTopBehavior() {
    _movementState = STOPPED;
    _logicState = SEEKING;
    _jiggerCount = 0;
    _lastFrontLocation = NOT_FOUND;
  }
  
  void initialize() {
    initializeMotorsAndEncoders(&_context);
    initializeEdgeSensors(&_context);
    initializeDistanceSensors(&_context);
    initializePixelRing(&_context);
  }

  void start(ButtonExecutor* buttonExecutor, PixelRing* pixelRing) {

    _buttonExecutor = buttonExecutor;
    
    // Start the motor controller
    _context.motorController->start();
    _context.pixelRing = pixelRing;

    // Get an initial read of the edge sensors
    readEdgeSensors((void*)&_context);
    
    // Register the methods used when running this behavior
    buttonExecutor->callbackEvery(50, readEdgeSensors, (void*)&_context);
    buttonExecutor->callbackEvery(10, adjustMotorSpeeds, (void*)&_context);
    buttonExecutor->callbackEvery(50, adjustPixelRing, (void*)&_context);
    buttonExecutor->callbackEvery(10, runBehavior, (void*)this);
  }
  
  void stop() {
    // Stop the robot
    _context.motorController->stop();
    _movementState = STOPPED;
    _logicState = SEEKING;
    _lastFrontLocation = NOT_FOUND;
    _jiggerCount = 0;
  }
  
  // Defines the behavior of the robot, a set of states that are cycled
  // through as sensor data is interpreted.
  void doBehavior() {
    if (_movementState != STOPPED && _movementState != TURN_F_LEFT && _movementState != TURN_F_RIGHT &&
        _movementState != TURN_R_LEFT && _movementState != TURN_R_RIGHT && _logicState != PUSHING_OFF) {
      if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
        if (_movementState != GOREVERSE) {
          DebugMsgs.println("detected edge: front left");
          if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
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
      } else if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
        if (_movementState != GOREVERSE) {
          DebugMsgs.println("detected edge: front right");
          if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
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
      } else if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
        if (_movementState != GOFORWARD) {
          DebugMsgs.println("detected edge: rear left");
          if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
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
      } else if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
        if (_movementState != GOFORWARD) {
          DebugMsgs.println("detected edge: rear right");
          if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
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
        if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
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
        if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
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
        if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
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
        if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
          if (_logicState == ACQUIRED || _logicState == PUSHING_OFF) {
            stopMovement();
          } else {
            goForward(150);
          }
        }
      }
      break;
      
      case GOFORWARD: {
        if (_targetTicks != 0 && _context.motorManager->readEncoder(RIGHT_MOTOR) >= _targetTicks) {
          stopMovement();
        }
        if (_logicState != LOCATED) {
          return;
        }
      }
      break;

      case GOREVERSE: {
        if (_targetTicks != 0 && _context.motorManager->readEncoder(RIGHT_MOTOR) <= _targetTicks) {
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
        _buttonExecutor->abortExecution();
        break;
    }
  }

  void handleSeekingState(void) {
    
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
        unsigned int currentTicks = _context.motorManager->readEncoder(_movementState == SPIN_LEFT ? RIGHT_MOTOR : LEFT_MOTOR);
        if (currentTicks >= _targetTicks) {
          stopMovement();
        }
      }
      break;
        
      default:
        break;
    }
  }

  void handleSpinSeekingState(void) {
    switch(_movementState) {
      case STOPPED: {
        spin(-190);
      }
      break;

      case SPIN_LEFT: {
        if (getDistanceFromObject(FRONT, LEFT) < DISTANCE_THRESHOLD) {
          DebugMsgs.println("Spin seeking front left found object, switching to seeking");
          stopMovement();
          _logicState = SEEKING;
        } else if (getDistanceFromObject(REAR, LEFT) < DISTANCE_THRESHOLD) {
          DebugMsgs.println("Spin seeking rear left found object, switching to seeking");
          stopMovement();
          _logicState = SEEKING;
          // turn to the right to center on the object
          DebugMsgs.println("Spinning right to center on object");
          spin(170);
        } else {
          unsigned int currentTicks = _context.motorManager->readEncoder(RIGHT_MOTOR);
          if (currentTicks >= _targetTicks) {
            DebugMsgs.println("Spin seeking did not find object, switch back to seeking and go forward");
            _logicState = SEEKING;
            goForward(150);
          }
        }
      }
      break;
        
      case SPIN_RIGHT: {
        if (getDistanceFromObject(FRONT, RIGHT) < DISTANCE_THRESHOLD) {
          DebugMsgs.println("Spin seeking front right found object, switching to seeking");
          stopMovement();
          _logicState = SEEKING;
        } else if (getDistanceFromObject(REAR, RIGHT) < DISTANCE_THRESHOLD) {
          DebugMsgs.println("Spin seeking rear right found object, switching to seeking");
          stopMovement();
          _logicState = SEEKING;
          // turn to the left to center on the object
          DebugMsgs.println("Spinning left to center on object");
          spin(-170);
        } else {
          unsigned int currentTicks = _context.motorManager->readEncoder(LEFT_MOTOR);
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

  void handleLocatedState(void) {
    
    switch(_movementState) {
      case STOPPED: {
        int distance = getDistanceFromObject(FRONT, CENTER);
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

  void handleAcquiredState() {
    // If acqusition has been lost, then back up and go back to seeking
    if (getDistanceFromObject(FRONT, CENTER) >= ACQUISITION_THRESHOLD) {
      DebugMsgs.println("object pushed off the table");
      _logicState = CELEBRATE;
      goReverse(100);
      return;
    }

    if (_movementState == STOPPED) {
      if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD || 
            _context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
        DebugMsgs.println("At the edge, switching to pushing off");
        _logicState = PUSHING_OFF;
      } else {
        DebugMsgs.println("Moving foward to push off the object");
        goForward();
      }
    }
  }

  void handlePushingOff() {
    // If acqusition has been lost, then the object fell off the tabel
    if (getDistanceFromObject(FRONT, CENTER) >= ACQUISITION_THRESHOLD) {
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

  void handleCelebrate() {
    switch(_movementState) {
      case STOPPED: {
        spinFast(1080);
      }
      break;
      
      case SPIN_RIGHT: {
        if (_context.motorManager->readEncoder(LEFT_MOTOR) >= _targetTicks) {
          stopMovement();
          _logicState = DONE;
        }
      }
      break;

      default:
      break;
    }
  }
  
  FieldLocation adjustForJigger(FieldLocation inputFieldLocation) {
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

  unsigned int getDistanceFromObject(Orientation orientation, SensorLocation location) {
    VL6180I2CMux* sensorMux = 
      (orientation == FRONT) ? _context.frontDistanceSensorMux : _context.rearDistanceSensorMux;
    return min(DISTANCE_THRESHOLD, sensorMux->readDistance(location));
  }
  
  FieldLocation checkForObject(Orientation orientation) {

    unsigned int left = getDistanceFromObject(orientation, LEFT);
    unsigned int center = getDistanceFromObject(orientation, CENTER);
    unsigned int right = getDistanceFromObject(orientation, RIGHT);
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
};

class StayOnTableTopBehavior : public Behavior {

  public:
  StayOnTableTopBehavior() { _movementState = STOPPED; }
  
  void initialize() {
    initializeMotorsAndEncoders(&_context);
    initializeEdgeSensors(&_context);
    initializePixelRing(&_context);
  }

  void start(ButtonExecutor* buttonExecutor, PixelRing* pixelRing) {

    // Start the motor controller
    _context.motorController->start();
    _context.pixelRing = pixelRing;

    // Register the methods used when running this behavior
    buttonExecutor->callbackEvery(50, readEdgeSensors, (void*)&_context);
    buttonExecutor->callbackEvery(50, adjustMotorSpeeds, (void*)&_context);
    buttonExecutor->callbackEvery(50, adjustPixelRing, (void*)&_context);
    buttonExecutor->callbackEvery(100, runBehavior, (void*)this);
  }
  
  // Defines the behavior of the robot, a set of states that are cycled
  // through as sensor data is interpreted.
  void doBehavior() {
    switch (_movementState) {
      case STOPPED:
        goForward();
        break;
      case GOFORWARD:
        if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
          if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
            goReverse();
          } else {
            turnForwardRight();
          }
        } else if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
          if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
            goReverse();
          } else {
            turnForwardLeft();
          }
        }
        break;
      case GOREVERSE:
        if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
          if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
            goForward();
          } else {
            turnReverseLeft();
          }
        } else if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
          if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
            goForward();
          } else {
            turnReverseRight();
          }
        }
        break;
      case TURN_F_LEFT:
        if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
          goReverse();
        }
        break;
      case TURN_F_RIGHT:
        if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
          goReverse();
        }
        break;
      case TURN_R_LEFT:
        if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
          goForward();
        }
        break;
      case TURN_R_RIGHT:
        if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
          goForward();
        }
        break;
      default:
        break;
    }
  }
  
  void stop() {
    // Stop the robot
    _context.motorController->stop();
    _movementState = STOPPED;
  }
};

// This method is used to instantiate the behavior that
// should be used for the robot. Modify it to create
// the behavior you want to use.
Behavior* createBehavior() {
  Behavior* behavior = new PushOffTableTopBehavior();
  //Behavior* behavior = new StayOnTableTopBehavior();
  behavior->initialize();
  return behavior;
}

void Behavior::stopMovement() {
  _movementState = STOPPED;
  _targetTicks = 0;
  _context.motorController->stop();
  _context.motorManager->setMotorSpeeds(0,0);
  _context.motorManager->readAndResetEncoder(LEFT_MOTOR);
  _context.motorManager->readAndResetEncoder(RIGHT_MOTOR);
  _context.pixelRing->changeState(RING_WHITE_FADE);
  DebugMsgs.println("stopping");
}

void Behavior::spin(int degrees) {
  resetEncoders(&_context);
  _targetTicks = abs(degrees) * TICKS_PER_BASE_DEGREE;
  _context.motorController->stop();
  if (degrees < 0) {
    _movementState = SPIN_LEFT;
    _context.motorManager->setMotorSpeeds(-SPIN_SPEED, SPIN_SPEED);
    DebugMsgs.print("spinning left ");
  } else if (degrees > 0) {
    _movementState = SPIN_RIGHT;
    _context.motorManager->setMotorSpeeds(SPIN_SPEED, -SPIN_SPEED);
    DebugMsgs.print("spinning right ");
  }
  DebugMsgs.print(degrees).println(" degrees");
  _context.pixelRing->changeState(RING_BLUE_CW);
}

void Behavior::spinFast(int degrees) {
  resetEncoders(&_context);
  _targetTicks = abs(degrees) * TICKS_PER_BASE_DEGREE;
  _context.motorController->stop();
  if (degrees < 0) {
    _movementState = SPIN_LEFT;
    _context.motorManager->setMotorSpeeds(-CRUISE_SPEED, CRUISE_SPEED);
    DebugMsgs.print("spinning fast left ");
  } else if (degrees > 0) {
    _movementState = SPIN_RIGHT;
    _context.motorManager->setMotorSpeeds(CRUISE_SPEED, -CRUISE_SPEED);
    DebugMsgs.print("spinning fast right ");
  }
  DebugMsgs.print(degrees).println(" degrees");
  _context.pixelRing->changeState(RING_BLUE_CW);
}

void Behavior::goForward(unsigned int millimeters) {
  _movementState = GOFORWARD;
  _targetTicks = millimeters * TICKS_PER_MM;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(CRUISE_SPEED, CRUISE_SPEED);
  _context.pixelRing->changeState(RING_GREEN_CW);
  DebugMsgs.print("going forward");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void Behavior::goReverse(unsigned int millimeters) {
  _movementState = GOREVERSE;
  _targetTicks = millimeters * -TICKS_PER_MM;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(-CRUISE_SPEED, -CRUISE_SPEED);
  _context.pixelRing->changeState(RING_RED_CW);
  DebugMsgs.print("going reverse");
  if (_targetTicks != 0) {
    DebugMsgs.print(" ").print(millimeters).print(" mm (")
      .print(_targetTicks).println(" ticks)");
  } else {
    DebugMsgs.println(", no limit");
  }
}

void Behavior::turnForwardRight() {
  _movementState = TURN_F_RIGHT;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(0, TURN_SPEED);
  _context.pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn forward right");
}

void Behavior::turnForwardLeft() {
  _movementState = TURN_F_LEFT;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(TURN_SPEED, 0);
  _context.pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn forward left");
}

void Behavior::turnReverseLeft() {
  _movementState = TURN_R_LEFT;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(0, -TURN_SPEED);
  _context.pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn reverse left");
}

void Behavior::turnReverseRight() {
  _movementState = TURN_R_RIGHT;
  _context.motorController->start();
  _context.motorController->setDesiredSpeeds(-TURN_SPEED, 0);
  _context.pixelRing->changeState(RING_BLUE_CW);
  DebugMsgs.println("turn reverse right");
}
