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

// Arduino includes
#include <inttypes.h>

// My includes
#include <SerialDebug.h>
#include <PololuQik2s9v1MotorManager.h>
#include <MotorController.h>

// Local includes
#include "helper_methods.h"
#include "pin_assignments.h"
#include "robot_constants.h"

void initializeMotorsAndEncoders(CallbackContext* context) {
  // Setup the motor manager
  context->motorManager = 
    new PololuQik2s9v1MotorManager(
      POLOLU_QIK_TX_PIN, POLOLU_QIK_RX_PIN, POLOLU_QIK_RESET_PIN);

  // Setup encoders on the motor manager
  context->motorManager->setupEncoders(
    ENCODER_L_PHASE_A_PIN, ENCODER_L_PHASE_B_PIN,
    ENCODER_R_PHASE_A_PIN, ENCODER_R_PHASE_B_PIN);

 // Setup the motor controller
 context->motorController = new MotorController(context->motorManager,
    KP, KI, KD, 50, RADIANS_PER_TICK, MAX_RADIANS_PER_SECOND);
}

void initializeDistanceSensors(CallbackContext* context) {
  Wire.begin();
  Wire.setClock(400000);
  
  Wire1.begin();
  Wire1.setClock(400000);

  context->frontDistanceSensorMux = new VL6180I2CMux(&Wire1, 0x29, NUM_FRONT_DISTANCE_SENSORS,
    FRONT_MUX_PIN0,FRONT_MUX_PIN1,FRONT_MUX_PIN2);
  context->frontDistanceSensorMux->initializeSensors();
  
  context->rearDistanceSensorMux = new VL6180I2CMux(&Wire, 0x29, NUM_REAR_DISTANCE_SENSORS,
    REAR_MUX_PIN0,REAR_MUX_PIN1,REAR_MUX_PIN2);
  context->rearDistanceSensorMux->initializeSensors();
}

void initializeEdgeSensors(CallbackContext* context) {
  
  context->edgeSensors = new QTRSensors();
  context->edgeSensors->setTypeRC();
  context->edgeSensors->setSensorPins((const uint8_t[]){
    FL_EDGE_SENSOR_PIN, FR_EDGE_SENSOR_PIN,
    RL_EDGE_SENSOR_PIN, RR_EDGE_SENSOR_PIN},
    NUM_EDGE_SENSORS);
}

void initializeSurfaceSensors(CallbackContext* context) {

  context->frontSurfaceSensors = new QTRSensors();
  context->frontSurfaceSensors->setTypeRC();
  context->frontSurfaceSensors->setSensorPins((const uint8_t[]){
    FL_SURFACE_SENSOR_PIN, FR_SURFACE_SENSOR_PIN},
    NUM_FRONT_SURFACE_SENSORS);

  context->centerSurfaceSensors = new QTRSensors();
  context->centerSurfaceSensors->setTypeRC();
  context->centerSurfaceSensors->setSensorPins((const uint8_t[]){
    CTR_SURFACE_SENSOR_1_PIN, CTR_SURFACE_SENSOR_2_PIN, CTR_SURFACE_SENSOR_3_PIN,
    CTR_SURFACE_SENSOR_4_PIN, CTR_SURFACE_SENSOR_5_PIN, CTR_SURFACE_SENSOR_6_PIN},
    NUM_CENTER_SURFACE_SENSORS);
    
  context->rearSurfaceSensors = new QTRSensors();
  context->rearSurfaceSensors->setTypeRC();
  context->rearSurfaceSensors->setSensorPins((const uint8_t[]){
    RL_SURFACE_SENSOR_PIN, RR_SURFACE_SENSOR_PIN},
    NUM_REAR_SURFACE_SENSORS);
}

void initializePixelRing(CallbackContext* context){
  context->pixelRing = new PixelRing(PIXEL_RING_PIN, NUM_PIXELS_ON_RING);
}

void resetEncoders(CallbackContext* context) {  
  // Reset the encoders
  context->motorManager->readAndResetEncoder(LEFT_MOTOR);
  context->motorManager->readAndResetEncoder(RIGHT_MOTOR);
}

void adjustMotorSpeeds(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);

  callbackContext->motorController->adjustSpeeds();
}

void readEdgeSensors(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);
  
  // read raw sensor values
  callbackContext->edgeSensors->read(callbackContext->edgeSensorValues);

//  SerialDebugger.print("FL: ").print(callbackContext->edgeSensorValues[FL_EDGE] > 200 ? "EDGE" : "TBLE").print("  ");
//  SerialDebugger.print("FR: ").println(callbackContext->edgeSensorValues[FR_EDGE] > 200 ? "EDGE" : "TBLE");
//  SerialDebugger.print("RL: ").print(callbackContext->edgeSensorValues[RL_EDGE] > 200 ? "EDGE" : "TBLE").print("  ");
//  SerialDebugger.print("RR: ").println(callbackContext->edgeSensorValues[RR_EDGE] > 200 ? "EDGE" : "TBLE");
//  SerialDebugger.println();
}

void readSurfaceSensors(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);
  
  callbackContext->frontSurfaceSensors->read(callbackContext->frontSurfaceSensorValues);
  callbackContext->centerSurfaceSensors->read(callbackContext->centerSurfaceSensorValues);
  callbackContext->rearSurfaceSensors->read(callbackContext->rearSurfaceSensorValues);
  
//  SerialDebugger.print("front: ").print(frontSurfaceSensorValues[0]).print('\t');
//  SerialDebugger.print(frontSurfaceSensorValues[1]).println();
//  SerialDebugger.print("center:").print(centerSurfaceSensorValues[0]).print('\t');
//  SerialDebugger.print(centerSurfaceSensorValues[1]).print('\t');
//  SerialDebugger.print(centerSurfaceSensorValues[2]).print('\t');
//  SerialDebugger.print(centerSurfaceSensorValues[3]).print('\t');
//  SerialDebugger.print(centerSurfaceSensorValues[4]).print('\t');
//  SerialDebugger.print(centerSurfaceSensorValues[5]).println();
//  SerialDebugger.print("rear:  ").print(rearSurfaceSensorValues[0]).print('\t');
//  SerialDebugger.print(rearSurfaceSensorValues[1]).println();
//  SerialDebugger.println();
}

void readDistanceSensors(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);
  
  SerialDebugger.print("front: ");
  for (uint8_t x = 0; x < NUM_FRONT_DISTANCE_SENSORS; x++) {
    SerialDebugger.print(x).print(": ").print(callbackContext->frontDistanceSensorMux->readDistance(x)).print(", ");
  }
  SerialDebugger.println();
  SerialDebugger.print("rear:  ");
  for (int8_t x = 0; x < NUM_REAR_DISTANCE_SENSORS; x++) {
    SerialDebugger.print(x).print(": ").print(callbackContext->rearDistanceSensorMux->readDistance(x)).print(", ");
  }
  SerialDebugger.println().println();
}

void adjustPixelRing(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);
  
  callbackContext->pixelRing->run();
}

void calibrateSurfaceSensors(CallbackContext* context) {
  digitalWrite(LED_PIN, HIGH); // turn on LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 200; i++)
  {
    context->frontSurfaceSensors->calibrate();
    context->centerSurfaceSensors->calibrate();
    context->rearSurfaceSensors->calibrate();
  }
  
  digitalWrite(LED_PIN, LOW); // flicker led to show we are half way done
  delay(50);
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
  delay(50);
  digitalWrite(LED_PIN, HIGH);
  
  for (uint16_t i = 0; i < 200; i++)
  {
    context->frontSurfaceSensors->calibrate();
    context->centerSurfaceSensors->calibrate();
    context->rearSurfaceSensors->calibrate();
  }
  digitalWrite(LED_PIN, LOW); // turn off led to indicate calibration done.
}

void stopMotors(CallbackContext* context) {
  context->motorManager->setMotorSpeeds(0, 0);
}