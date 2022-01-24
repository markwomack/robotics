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

const uint8_t LED_PIN              = 13; // led (13 is built-in)
const uint8_t SURFACE_SENSOR_1_PIN = 29; // surface sensor 1
const uint8_t SURFACE_SENSOR_2_PIN = 28; // surface sensor 2
const uint8_t SURFACE_SENSOR_3_PIN = 27; // surface sensor 3
const uint8_t SURFACE_SENSOR_4_PIN = 26; // surface sensor 4
const uint8_t SURFACE_SENSOR_5_PIN = 25; // surface sensor 5
const uint8_t SURFACE_SENSOR_6_PIN = 24; // surface sensor 6
const uint8_t RL_SURFACE_SENSOR_PIN = 23; // rear-left surface sensor
const uint8_t RR_SURFACE_SENSOR_PIN = 22; // rear-right surface sensor
const uint8_t FL_SURFACE_SENSOR_PIN = 20; // front-left surface sensor
const uint8_t FR_SURFACE_SENSOR_PIN = 21; // front-right surface sensor
const uint8_t BUTTON_PIN           = 33; // start/stop button

// Data structure used to store state between callback calls.
struct CallbackContext {
  char ledState;
};

CallbackContext _callbackContext;

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

QTRSensors qtr;
const uint8_t SensorCount = 10;
uint16_t sensorValues[SensorCount];

void setup() {
  SerialDebugger.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);

  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){
    FL_SURFACE_SENSOR_PIN, FR_SURFACE_SENSOR_PIN,
    SURFACE_SENSOR_1_PIN, SURFACE_SENSOR_2_PIN, SURFACE_SENSOR_3_PIN,
    SURFACE_SENSOR_4_PIN, SURFACE_SENSOR_5_PIN, SURFACE_SENSOR_6_PIN,
    RL_SURFACE_SENSOR_PIN, RR_SURFACE_SENSOR_PIN
    }, SensorCount);
    
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

  digitalWrite(LED_PIN, HIGH); // turn on LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 200; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_PIN, LOW); // flick led to show we are half way done
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  for (uint16_t i = 0; i < 200; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_PIN, LOW); // turn off led to indicate calibration done.
  
  SerialDebugger.println();for (uint8_t i = 0; i < SensorCount; i++)
  {
    SerialDebugger.print(qtr.calibrationOn.minimum[i]).print(' ');
  }
  SerialDebugger.println();

  // print the calibration maximum values measured when emitters were on
  SerialDebugger.println();for (uint8_t i = 0; i < SensorCount; i++)
  {
    SerialDebugger.print(qtr.calibrationOn.maximum[i]).print(' ');
  }
  SerialDebugger.println();
 
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
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  //uint16_t position = qtr.readLineBlack(sensorValues);
  
  // read raw sensor values
  qtr.read(sensorValues);
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  SerialDebugger.print(sensorValues[0]).print('\t');
  SerialDebugger.print(sensorValues[1]).println();
  SerialDebugger.print(sensorValues[2]).print('\t');
  SerialDebugger.print(sensorValues[3]).print('\t');
  SerialDebugger.print(sensorValues[4]).print('\t');
  SerialDebugger.print(sensorValues[5]).print('\t');
  SerialDebugger.print(sensorValues[6]).print('\t');
  SerialDebugger.print(sensorValues[7]).println();
  SerialDebugger.print(sensorValues[8]).print('\t');
  SerialDebugger.print(sensorValues[9]).println();
}

void applyLEDState(void* context) {
  // change led state
  char ledState = (((CallbackContext*)context)->ledState == LOW ? HIGH : LOW);
  digitalWrite(LED_PIN, ledState);

  // store state for next callback
  ((CallbackContext*)context)->ledState = ledState;
}
