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

// Teensy includes
#include <WireIMXRT.h> // Teensy Wire replacement

// Project includes
#include <ButtonExecutor.h>
#include <SerialDebug.h>
#include <VL6180I2CMux.h>

const uint8_t REAR_MUX_PIN0     = 10; // rear_sensor mux selector pin 0
const uint8_t REAR_MUX_PIN1     = 11; // rear_sensor mux selector pin 1
const uint8_t REAR_MUX_PIN2     = 12; // rear_sensor mux selector pin 2
const uint8_t LED_PIN           = 13; // led (13 is built-in)
const uint8_t FRONT_MUX_PIN2    = 14; // front sensor mux selector pin 2
const uint8_t FRONT_MUX_PIN1    = 15; // front sensor mux selector pin 1
const uint8_t FRONT_MUX_PIN0    = 16; // front sensor mux selector pin 0
const uint8_t BUTTON_PIN        = 33; // start/stop button

// Create the button executor that will start/stop execution when button
// is pressed.
ButtonExecutor buttonExecutor;

VL6180I2CMux* frontSensorMux;
int numFrontMuxSensor = 5;

VL6180I2CMux* rearSensorMux;
int numRearMuxSensor = 4;

void setup() {
  SerialDebugger.begin(9600);
  
  Wire.begin();
  Wire.setClock(400000);
  
  Wire1.begin();
  Wire1.setClock(400000);

  pinMode(LED_PIN, OUTPUT);

  frontSensorMux = new VL6180I2CMux(&Wire, 0x29, numFrontMuxSensor,
    FRONT_MUX_PIN0,FRONT_MUX_PIN1,FRONT_MUX_PIN2);
  frontSensorMux->initializeSensors();
  
  rearSensorMux = new VL6180I2CMux(&Wire1, 0x29, numRearMuxSensor,
    REAR_MUX_PIN0,REAR_MUX_PIN1,REAR_MUX_PIN2);
  rearSensorMux->initializeSensors();

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
}

// This is where the sketch should set initial state before execution.  It will
// be called every time the button is pushed to start execution, before
// execution is started.
void startCallback() {
  SerialDebugger.println("Sketch start");
 
  // Read sensor values every second
  buttonExecutor.callbackEvery(1000, readSensors, (void*)0);
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
  SerialDebugger.print("front: ");
  for (uint8_t x = 0; x < numFrontMuxSensor; x++) {
    SerialDebugger.print(x).print(": ").print(frontSensorMux->readDistance(x)).print(", ");
  }
  SerialDebugger.println();
  SerialDebugger.print("rear:  ");
  for (int8_t x = 0; x < numRearMuxSensor; x++) {
    SerialDebugger.print(x).print(": ").print(rearSensorMux->readDistance(x)).print(", ");
  }
  SerialDebugger.println().println();
}
