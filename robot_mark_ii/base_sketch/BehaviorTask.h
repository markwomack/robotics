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
#include "MotorsAndEncoders.h"
#include "PixelRing.h"
#include "SurfaceSensors.h"

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
    BehaviorTask() { };
    void setEdgeSensors(EdgeSensors* edgeSensors);
    void setDistanceSensors(DistanceSensors* distanceSensors);
    void setMotorsAndEncoders(MotorsAndEncoders* motorsAndEncoders);
    void setPixelRing(PixelRing* pixelRing);
    void setSurfaceSensors(SurfaceSensors* surfaceSensors);
    void setTaskToken(uint8_t taskToken);

    virtual void setup(void) { };
    virtual void start(void) { };
    virtual void update(void) { };
    virtual void stop(void) { };

  protected:
    uint8_t _taskToken;
    EdgeSensors* _edgeSensors;
    DistanceSensors* _distanceSensors;
    MotorsAndEncoders* _motorsAndEncoders;
    PixelRing* _pixelRing;
    SurfaceSensors* _surfaceSensors;

    MovementState _movementState;
    double _targetTicks;

    // Shared movement methods
    void stopMovement();
    void spin(int degrees);
    void spinFast(int degrees);
    void goForward(unsigned int millimeters = 0);
    void goReverse(unsigned int millimeters = 0);
    void turnForwardRight();
    void turnForwardLeft();
    void turnReverseLeft();
    void turnReverseRight();
};

#endif //BEHAVIORTASK_H
