/*
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

#ifndef ButtonExecutor_h
#define ButtonExecutor_h

#include <Arduino.h>
#include <inttypes.h>
#include "Timer.h"

#define CALLBACK_NOT_INSTALLED TIMER_NOT_AN_EVENT

// State stored and sent to the executor callbacks
struct ExecutorContext {
	Timer timer;
	int8_t buttonPin;
  char buttonDefaultState;
	char oldButtonState;
	boolean isExecuting;
	void (*sketchStartCallback)();
	void (*sketchStopCallback)();
	int8_t callbackReferences[MAX_NUMBER_OF_EVENTS];
};

/*
 * Use this class to monitor a button that will call start and stop
 * execution when pressed. Callbacks can be registered to execute
 * periodically while the executing.
 */
class ButtonExecutor {

public:
	ButtonExecutor(void);

  /* 
   *  Sets up the executor with the callbacks and the pin to monitor for state change.
   *  
   * buttonPin - The pin to monitor for change in button state.
   * buttonDefaultState - The default state of the button. Button can be pulled HIGH
   *   or LOW.
   * sketchSetupCallback - The method to call with initial setup. Only called once.
   * sketchStartCallback - The method called when button pressed to start.
   * sketchStopCallback - The method called when button pressed to stop.
   */
	void setup(int8_t buttonPin, char buttonDefaultState, void (*sketchSetupCallback)(),
		void (*sketchStartCallback)(), void (*sketchStopCallback)());

  /*
   * Call this in the loop method of the sketch. As part of its execution, it will
   * make calls to registered callbacks if it is time to call them.
   */
	void loop();

  /*
   * Use to register callback methods that should be called after the button has
   * been pressed to start.
   * 
   * period - The period that the callback should be called in milliseconds. For
   *   example, a value of 1000 would call the callback every second.
   * callback - The callback method which takes a void* parameter. The void* will
   *   be a pointer to the context as passed in the context parameter.
   * context - A pointer to a struct that will be passed to the callback method
   *   each time it is called. Allows for data to be stored between calls. It is
   *   up to the callback method to convert it to the proper pointer to access
   *   the data.
   *   
   * Returns a callback id that uniquely identifies the registered callback.
   * This value can be passed to callbackStop to remove the callback
   * from execution.
   */
	int8_t callbackEvery(unsigned long period, void (*callback)(void*),
		void* context);

  /*
   * Will remove the callback that matches the id from future execution.
   * 
   * Returns TIMER_NOT_AN_EVENT if the callbackId was valid and the callback
   * was removed. Returns CALLBACK_NOT_INSTALLED if the callbackId was not
   * valid.
   */
	int8_t callbackStop(int8_t callbackId);

  /*
   * Can be called anywhere in the sketch to abort execution of the callbacks.
   * The registered sketchStopCallback method will be called. For example, a
   * registered callback may encounter an error condition that requires that
   * everything stop. It would call abortExecution to do this.
   */
	void abortExecution();
	
protected:
	ExecutorContext _executorContext;
};

#endif
