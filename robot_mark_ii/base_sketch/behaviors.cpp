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

// Local includes
#include "behaviors.h"
#include "robot_constants.h"
#include "helper_methods.h"

enum State {
  STOPPED,
  FORWARD,
  REVERSE,
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

class BehaviorWithState : public Behavior {
  public:
    BehaviorWithState() { }
    
    virtual void initialize();
    virtual void start(ButtonExecutor* buttonExecutor);
    virtual void doBehavior();
    virtual void stop();

  protected:
    State _state;
};

class TableTopBehavior : public BehaviorWithState {
  public:
  TableTopBehavior() { _state = STOPPED; }
  
  void initialize() {
    initializeMotorsAndEncoders(&_context);
    initializeEdgeSensors(&_context);
  }

  void start(ButtonExecutor* buttonExecutor) {

    // Reset the encoder and velocities in the callback context
    resetEncodersAndVelocities(&_context);
    readEncoders(&_context);

    // Register the methods used when running this behavior
    buttonExecutor->callbackEvery(50, readEdgeSensors, (void*)&_context);
    buttonExecutor->callbackEvery(100, runBehavior, (void*)this);
  }

  // Defines the behavior of the robot, a set of states that are cycled
  // through as sensor data is interpreted.
  void doBehavior() {
    switch (_state) {
      case STOPPED:
        _state = FORWARD;
        goForward(&_context);
        break;
      case FORWARD:
        if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
          _state = TURN_F_RIGHT;
          turnForwardRight(&_context);
        } else if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
          _state = TURN_F_LEFT;
          turnForwardLeft(&_context);
        }
        break;
      case REVERSE:
        if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
          _state = TURN_R_LEFT;
          turnReverseLeft(&_context);
        } else if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
          _state = TURN_R_RIGHT;
          turnReverseRight(&_context);
        }
        break;
      case TURN_F_LEFT:
        if (_context.edgeSensorValues[FL_EDGE] > EDGE_THRESHOLD) {
          _state = REVERSE;
          goReverse(&_context);
        }
        break;
      case TURN_F_RIGHT:
        if (_context.edgeSensorValues[FR_EDGE] > EDGE_THRESHOLD) {
          _state = REVERSE;
          goReverse(&_context);
        }
        break;
      case TURN_R_LEFT:
        if (_context.edgeSensorValues[RR_EDGE] > EDGE_THRESHOLD) {
          _state = FORWARD;
          goForward(&_context);
        }
        break;
      case TURN_R_RIGHT:
        if (_context.edgeSensorValues[RL_EDGE] > EDGE_THRESHOLD) {
          _state = FORWARD;
          goForward(&_context);
        }
        break;
      default:
        break;
    }
  }
  
  void stop() {
    
    // Stop the robot
    stopMotors(&_context);
    _state = STOPPED;
  }
};

// This method is used to instantiate the behavior that
// should be used for the robot. Modify it to create
// the behavior you want to use.
Behavior* createBehavior() {
  Behavior* behavior = new TableTopBehavior();
  return behavior;
}
