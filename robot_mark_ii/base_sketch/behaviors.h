//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
#ifndef BEHAVIORS_H
#define BEHAVIORS_H

// My includes
#include <TaskManager.h>

// Local includes
#include "callback_context.h"
#include "pixel_ring.h"

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

class Behavior {
  public:
    Behavior() { }
    
    virtual void initialize();
    virtual void start(TaskManager* taskManager, PixelRing* pixelRing);
    virtual void doBehavior();
    virtual void stop();

  protected:
    CallbackContext _context;
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

Behavior* createBehavior();

#endif
