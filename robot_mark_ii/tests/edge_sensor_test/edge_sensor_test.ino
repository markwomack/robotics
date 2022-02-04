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

#include <QTRSensors.h>

// Project includes
#include <ButtonExecutor.h>
#include <SerialDebug.h>

const uint8_t RR_EDGE_SENSOR_PIN =  2; // rear-right edge sensor
const uint8_t LED_PIN            = 13; // led (13 is built-in)
const uint8_t RL_EDGE_SENSOR_PIN = 17; // rear-left edge sensor
const uint8_t FR_EDGE_SENSOR_PIN = 30; // front-right edge sensor
const uint8_t BUTTON_PIN         = 33; // start/stop button
const uint8_t FL_EDGE_SENSOR_PIN = 36; // front_left edge sensor

const uint8_t FL = 0;
const uint8_t FR = 1;
const uint8_t RL = 2;
const uint8_t RR = 3;

// Data structure used to store state between callback calls.
struct CallbackContext {
  char ledState;
};

CallbackContext _callbackContext;

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

QTRSensors qtr;
const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];

void setup() {
  SerialDebugger.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);

  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){
    FL_EDGE_SENSOR_PIN, FR_EDGE_SENSOR_PIN,
    RL_EDGE_SENSOR_PIN, RR_EDGE_SENSOR_PIN}, SensorCount);

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
  
  // Intialize the callback state
  _callbackContext.ledState = LOW;
}

// This is where the sketch should set initial state before execution.  It will
// be called every time the button is pushed to start execution, before
// execution is started.
void startCallback() {
  SerialDebugger.println("Sketch start");
  
  // Read sensor values every second
  buttonExecutor.callbackEvery(500, applyLEDState, (void*)&_callbackContext);
  buttonExecutor.callbackEvery(500, readSensors, (void*)0);
}

// This is where the sketch should handle the ending of execution.  It will be
// called whenever execution is ended (button push or request to abort
// execution).
void stopCallback() {
  SerialDebugger.println("Sketch stop");

  // Turn off the led
  digitalWrite(LED_PIN, LOW);
}

void abortExecution() {
  buttonExecutor.abortExecution();
}

void readSensors(void* context) {
  // read raw sensor values
  qtr.read(sensorValues);

  SerialDebugger.print("FL: ").print(sensorValues[FL] > 200 ? "EDGE" : "TBLE").print("  ");
  SerialDebugger.print("FR: ").println(sensorValues[FR] > 200 ? "EDGE" : "TBLE");
  SerialDebugger.print("RL: ").print(sensorValues[RL] > 200 ? "EDGE" : "TBLE").print("  ");
  SerialDebugger.print("RR: ").println(sensorValues[RR] > 200 ? "EDGE" : "TBLE");
  SerialDebugger.println();
}

void applyLEDState(void* context) {
  // change led state
  char ledState = (((CallbackContext*)context)->ledState == LOW ? HIGH : LOW);
  digitalWrite(LED_PIN, ledState);

  // store state for next callback
  ((CallbackContext*)context)->ledState = ledState;
}
