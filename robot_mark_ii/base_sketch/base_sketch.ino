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

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// My includes
#include <ButtonExecutor.h>
#include <SerialDebug.h>

// Local includes
#include "pin_assignments.h"
#include "behaviors.h"

// This is the state used by the led callback
struct LEDContext {
  char ledState;
};
LEDContext ledContext;

// This is the behavior defined for the robot
Behavior* behavior;

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

void setup() {
  SerialDebugger.begin(9600);
    
  // Setup the executor with the button pin and callbacks
  buttonExecutor.setup(
    BUTTON_PIN, HIGH, setupCallback, startCallback, stopCallback);
}

void loop() {
  // Allow the executor to perform a loop call to check for button presses
  // and to execute the sketch callbacks
  buttonExecutor.loop();
}

// This is where the sketch should setup one time settings like pin modes.
// It will be called just once when the executor is setup.
void setupCallback() {
  SerialDebugger.println("Sketch setup");

  // Intialize the led callback state
  pinMode(LED_PIN, OUTPUT);
  ledContext.ledState = LOW;

  // Create and initialize the behavior
  behavior = createBehavior();
  behavior->initialize();
}

// This is where the sketch should set initial state before execution.  It will
// be called every time the button is pushed to start execution, before
// execution is started.
void startCallback() {
  SerialDebugger.println("Sketch start");

  // blink the led every half second
  buttonExecutor.callbackEvery(500, applyLEDState, (void*)&ledContext);

  // Start the behavior
  behavior->start(&buttonExecutor);
}

// This is where the sketch should handle the ending of execution.  It will be
// called whenever execution is ended (button push or request to abort
// execution).
void stopCallback() {
  SerialDebugger.println("Sketch stop");

  // Turn off the led, set everything to off
  ledContext.ledState = LOW;
  digitalWrite(LED_PIN, LOW);

  // Stop the behavior
  behavior->stop();
}

void abortExecution() {
  buttonExecutor.abortExecution();
}

void applyLEDState(void* context) {
  // change led state
  char ledState = ((LEDContext*)context)->ledState == LOW ? HIGH : LOW;
  digitalWrite(LED_PIN, ledState);

  // store state for next callback
  ((LEDContext*)context)->ledState = ledState;
}
