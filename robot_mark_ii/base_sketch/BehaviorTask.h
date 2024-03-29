//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef BEHAVIORTASK_H
#define BEHAVIORTASK_H

// My includes
#include <Task.h>

// Local includes
#include "EdgeSensors.h"
#include "DistanceSensors.h"
#include "SurfaceSensors.h"
#include "MotorsAndEncoders.h"
#include "Animation.h"

enum MovementState {
  STOPPED,
  GOFORWARD,
  GOREVERSE,
  TURN_F_LEFT,
  TURN_F_RIGHT,
  TURN_R_LEFT,
  TURN_R_RIGHT,
  SPIN_LEFT,
  SPIN_RIGHT
};

class BehaviorTask : public Task {
  public:
    BehaviorTask();
    void setTaskToken(uint8_t taskToken);

    virtual void setup(void) { };
    virtual void start(void) { };
    virtual void update(void) { };
    virtual void stop(void) { };

  protected:
    uint8_t _taskToken;
    EdgeSensors* _edgeSensors;
    DistanceSensors* _distanceSensors;
    SurfaceSensors* _surfaceSensors;
    MotorsAndEncoders* _motorsAndEncoders;
    Animation* _animation;

    MovementState _movementState;
    double _targetTicks;

    // Shared movement methods
    void stopMovement();
    void spin(double spinSpeed, int degrees);
    void spin(int degrees);
    void spinFast(int degrees);
    void goForward(double leftSpeed, double rightSpeed, uint32_t millimeters = 0);
    void goForward(uint32_t millimeters = 0);
    void goReverse(double leftSpeed, double rightSpeed, uint32_t millimeters = 0);
    void goReverse(uint32_t millimeters = 0);
    void turnForwardRight();
    void turnForwardLeft();
    void turnReverseLeft();
    void turnReverseRight();
};

#endif //BEHAVIORTASK_H
