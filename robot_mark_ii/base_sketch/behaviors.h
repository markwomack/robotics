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
