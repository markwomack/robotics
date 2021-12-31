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

// Teensy library includes
#include <Encoder.h>

// Project includes
#include <ButtonExecutor.h>
#include <SerialDebug.h>
//#include "../../ard_lib/Timer.h"
#include <PololuQik2s9v1MotorManager.h>

// Pin Definitions - Defines all of the pins used
#define POLOLU_QIK_TX_PIN       0 // tx pin to communicate with Pololu qik
#define POLOLU_QIK_RX_PIN       1 // rx pin to communicate with Pololu qik
#define POLOLU_QIK_RESET_PIN    3 // reset pin to communicate with Pololu qik
#define LED_PIN                13 // pin to flash an led (13 is built-in)
#define ENCODER_RT_PHASE_B_PIN 31 // pin to detect right encoder phase b signal
#define ENCODER_RT_PHASE_A_PIN 32 // pin to detect right encoder phase a signal
#define BUTTON_PIN             33 // pin to detect the start/stop button
#define ENCODER_LT_PHASE_B_PIN 34 // pin to detect left encoder phase b signal
#define ENCODER_LT_PHASE_A_PIN 35 // pin to detect left encoder phase a signal

// Data structure used to store state between callback calls.
struct CallbackContext {
  boolean direction;
  char ledState;
};

CallbackContext _callbackContext;

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

// Create the motor manager
MotorManager* motorManager;

void setup() {
	SerialDebugger.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  
	// Setup the executor with the button pin and callbacks
	buttonExecutor.setup(
		BUTTON_PIN, HIGH, setupCallback, startCallback, stopCallback);

  // Setup the motor manager
  motorManager = 
    new PololuQik2s9v1MotorManager(
      POLOLU_QIK_TX_PIN, POLOLU_QIK_RX_PIN, POLOLU_QIK_RESET_PIN);
    
  // Setup encoders on the motor manager
  motorManager->setupEncoders(
    ENCODER_LT_PHASE_A_PIN, ENCODER_LT_PHASE_B_PIN,
    ENCODER_RT_PHASE_A_PIN, ENCODER_RT_PHASE_B_PIN);
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

  // Intialize the callback state
  _callbackContext.direction = false;
  _callbackContext.ledState = LOW;
}

// This is where the sketch should set initial state before execution.  It will
// be called every time the button is pushed to start execution, before
// execution is started.
void startCallback() {

  // Initial callbacks to start
  checkEncoderValues((void*)&_callbackContext); // Initial read before starting motors
  applyMotorState((void*)&_callbackContext); // Start the motors!

  // Then make these callbacks on timed schedule
  buttonExecutor.callbackEvery(1000, applyMotorState, (void*)&_callbackContext);
  buttonExecutor.callbackEvery(500, applyLEDState, (void*)&_callbackContext);
  buttonExecutor.callbackEvery(10, checkEncoderValues, (void*)&_callbackContext);
}

// This is where the sketch should handle the ending of execution.  It will be
// called whenever execution is ended (button push or request to abort
// execution).
void stopCallback() {
	SerialDebugger.println("Sketch stop");

  // Turn off the led
  digitalWrite(LED_PIN, LOW);
  
  // Stop the motors
  motorManager->setMotorSpeeds(0,0);

  // Reset the encoders
  motorManager->readAndResetEncoder(LEFT_MOTOR);
  motorManager->readAndResetEncoder(RIGHT_MOTOR);
}

void abortExecution() {
	buttonExecutor.abortExecution();
}

// This method applies any changed motor state
void applyMotorState(void* context) {
  // change direction
  bool direction = !((CallbackContext*)context)->direction;
  int motorSpeed = (direction ? MAX_MOTORSPEED : -MAX_MOTORSPEED)/2;
  //motorManager->setMotorSpeed(RIGHT_MOTOR, motorSpeed);
  motorManager->setMotorSpeeds(motorSpeed, -motorSpeed);
  
  // store state for next callback
  ((CallbackContext*)context)->direction = direction;
}

void applyLEDState(void* context) {
  // change led state
  char ledState = (((CallbackContext*)context)->ledState == LOW ? HIGH : LOW);
  digitalWrite(LED_PIN, ledState);

  // store state for next callback
  ((CallbackContext*)context)->ledState = ledState;
}

void checkEncoderValues(void* context) {
  SerialDebugger.print("rightEncoder: ").println(motorManager->readEncoder(RIGHT_MOTOR));
  SerialDebugger.print("leftEncoder:  ").println(motorManager->readEncoder(LEFT_MOTOR));
}
