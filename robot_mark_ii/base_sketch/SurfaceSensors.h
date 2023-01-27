//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef SURFACESENSORS_H
#define SURFACESENSORS_H

// Third party includes
#include <QTRSensors.h>

// My includes
#include <DebugMsgs.h>

// Local includes
#include "pin_assignments.h"
#include "robot_constants.h"


// THIS IS A WORK IN PROGRESS
class SurfaceSensors {
  public:
    SurfaceSensors() {};

    void initialize(void) {
      _frontSurfaceSensors = new QTRSensors();
      _frontSurfaceSensors->setTypeRC();
      _frontSurfaceSensors->setSensorPins((const uint8_t[]){
        FL_SURFACE_SENSOR_PIN, FR_SURFACE_SENSOR_PIN},
        NUM_FRONT_SURFACE_SENSORS);
    
      _centerSurfaceSensors = new QTRSensors();
      _centerSurfaceSensors->setTypeRC();
      _centerSurfaceSensors->setSensorPins((const uint8_t[]){
        CTR_SURFACE_SENSOR_1_PIN, CTR_SURFACE_SENSOR_2_PIN, CTR_SURFACE_SENSOR_3_PIN,
        CTR_SURFACE_SENSOR_4_PIN, CTR_SURFACE_SENSOR_5_PIN, CTR_SURFACE_SENSOR_6_PIN},
        NUM_CENTER_SURFACE_SENSORS);
        
      _rearSurfaceSensors = new QTRSensors();
      _rearSurfaceSensors->setTypeRC();
      _rearSurfaceSensors->setSensorPins((const uint8_t[]){
        RL_SURFACE_SENSOR_PIN, RR_SURFACE_SENSOR_PIN},
        NUM_REAR_SURFACE_SENSORS);
    };

  private:
    QTRSensors* _centerSurfaceSensors;
    QTRSensors* _frontSurfaceSensors;
    QTRSensors* _rearSurfaceSensors;
};

/*
 void readSurfaceSensors(void* context) {
  CallbackContext* callbackContext = ((CallbackContext*)context);
  
  callbackContext->frontSurfaceSensors->read(callbackContext->frontSurfaceSensorValues);
  callbackContext->centerSurfaceSensors->read(callbackContext->centerSurfaceSensorValues);
  callbackContext->rearSurfaceSensors->read(callbackContext->rearSurfaceSensorValues);
  
//  DebugMsgs.print("front: ").print(frontSurfaceSensorValues[0]).print('\t');
//  DebugMsgs.print(frontSurfaceSensorValues[1]).println();
//  DebugMsgs.print("center:").print(centerSurfaceSensorValues[0]).print('\t');
//  DebugMsgs.print(centerSurfaceSensorValues[1]).print('\t');
//  DebugMsgs.print(centerSurfaceSensorValues[2]).print('\t');
//  DebugMsgs.print(centerSurfaceSensorValues[3]).print('\t');
//  DebugMsgs.print(centerSurfaceSensorValues[4]).print('\t');
//  DebugMsgs.print(centerSurfaceSensorValues[5]).println();
//  DebugMsgs.print("rear:  ").print(rearSurfaceSensorValues[0]).print('\t');
//  DebugMsgs.print(rearSurfaceSensorValues[1]).println();
//  DebugMsgs.println();
}

void calibrateSurfaceSensors(CallbackContext* context) {

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 200; i++)
  {
    context->frontSurfaceSensors->calibrate();
    context->centerSurfaceSensors->calibrate();
    context->rearSurfaceSensors->calibrate();
  }
  
  for (uint16_t i = 0; i < 200; i++)
  {
    context->frontSurfaceSensors->calibrate();
    context->centerSurfaceSensors->calibrate();
    context->rearSurfaceSensors->calibrate();
  }
}
 */

#endif //SURFACESENSORS_H
