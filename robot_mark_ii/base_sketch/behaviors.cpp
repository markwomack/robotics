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
#include <SerialDebug.h>
#include <MotorController.h>

// Local includes
#include "behaviors.h"
#include "robot_constants.h"
#include "helper_methods.h"

enum TableTopState {
  STOPPED,
  GOFORWARD,
  GOREVERSE,
  TURN_F_LEFT,
  TURN_F_RIGHT,
  TURN_R_LEFT,
  TURN_R_RIGHT
};

// Run the behavior stored in the context pointer.
void runBehavior(void* context) {  
  Behavior* behavior = (Behavior*)context;
  behavior->doBehavior();
}

class TableTopBehavior : public Behavior {
  protected:
    TableTopState _state;
    
  public:
  TableTopBehavior() { _state = STOPPED; }
  
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
    switch (_state) {
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
  
  void goForward() {
    _state = GOFORWARD;
    _context.motorController->setDesiredSpeeds(CRUISE_SPEED, CRUISE_SPEED);
    _context.pixelRing->changeState(RING_GREEN_CW);
    SerialDebugger.println("going forward");
  }

  void goReverse() {
    _state = GOREVERSE;
    _context.motorController->setDesiredSpeeds(-CRUISE_SPEED, -CRUISE_SPEED);
    _context.pixelRing->changeState(RING_RED_CW);
    SerialDebugger.println("going reverse");
  }

  void turnForwardRight() {
    _state = TURN_F_RIGHT;
    _context.motorController->setDesiredSpeeds(0, TURN_SPEED);
    _context.pixelRing->changeState(RING_BLUE_CW);
    SerialDebugger.println("turn forward right");
  }

  void turnForwardLeft() {
    _state = TURN_F_LEFT;
    _context.motorController->setDesiredSpeeds(TURN_SPEED, 0);
    _context.pixelRing->changeState(RING_BLUE_CW);
    SerialDebugger.println("turn forward left");
  }

  void turnReverseLeft() {
    _state = TURN_R_LEFT;
    _context.motorController->setDesiredSpeeds(0, -TURN_SPEED);
    _context.pixelRing->changeState(RING_BLUE_CW);
    SerialDebugger.println("turn reverse left");
  }

  void turnReverseRight() {
    _state = TURN_R_RIGHT;
    _context.motorController->setDesiredSpeeds(-TURN_SPEED, 0);
    _context.pixelRing->changeState(RING_BLUE_CW);
    SerialDebugger.println("turn reverse right");
  }
  
  void stop() {
    // Stop the robot
    _context.motorController->stop();
    _state = STOPPED;
  }
};

// This method is used to instantiate the behavior that
// should be used for the robot. Modify it to create
// the behavior you want to use.
Behavior* createBehavior() {
  Behavior* behavior = new TableTopBehavior();
  behavior->initialize();
  return behavior;
}
