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
#include <ButtonExecutor.h>

// Local includes
#include "callback_context.h"

class Behavior {
  public:
    Behavior() { }
    
    virtual void initialize();
    virtual void start(ButtonExecutor* buttonExecutor);
    virtual void doBehavior();
    virtual void stop();

  protected:
    CallbackContext _context;
};

Behavior* createBehavior();

#endif
