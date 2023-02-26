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
enum SurfaceSensorIndex {
  FL, FR,
  C0,C1,C2,C3,C4,C5,
  RL, RR
};

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

    void calibrate(void) {
      _frontSurfaceSensors->calibrate();
      _centerSurfaceSensors->calibrate();
      _rearSurfaceSensors->calibrate();
    };

    void read(void) {
      // read raw sensor values
      _frontSurfaceSensors->read(_frontSensorValues);
      _centerSurfaceSensors->read(_centerSensorValues);
      _rearSurfaceSensors->read(_rearSensorValues);
    };

    uint16_t value(SurfaceSensorIndex sensor) {
      switch(sensor) {
        case FL:
          return _frontSensorValues[0];
          
        case FR:
          return _frontSensorValues[1];
          
        case C0:
          return _centerSensorValues[0];
          
        case C1:
          return _centerSensorValues[1];
          
        case C2:
          return _centerSensorValues[2];
          
        case C3:
          return _centerSensorValues[3];
          
        case C4:
          return _centerSensorValues[4];
          
        case C5:
          return _centerSensorValues[5];

        case RL:
          return _rearSensorValues[0];
          
        case RR:
          return _rearSensorValues[1];
      }
    };

  private:
    QTRSensors* _frontSurfaceSensors;
    QTRSensors* _centerSurfaceSensors;
    QTRSensors* _rearSurfaceSensors;
    uint16_t _frontSensorValues[NUM_FRONT_SURFACE_SENSORS];
    uint16_t _centerSensorValues[NUM_CENTER_SURFACE_SENSORS];
    uint16_t _rearSensorValues[NUM_REAR_SURFACE_SENSORS];
};

#endif //SURFACESENSORS_H
