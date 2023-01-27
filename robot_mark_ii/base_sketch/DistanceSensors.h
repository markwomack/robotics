//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef DISTANCESENSORS_H
#define DISTANCESENSORS_H

// My includes
#include <DebugMsgs.h>
#include <VL6180I2CMux.h>

// Local includes
#include "pin_assignments.h"
#include "robot_constants.h"

enum SensorLocation {
  SIDE_LEFT = 0,
  LEFT = 1,
  CENTER = 2,
  RIGHT = 3,
  SIDE_RIGHT = 4
};

enum Orientation {
  FRONT,
  REAR
};

class DistanceSensors {
  public:
    DistanceSensors() {};

    void initialize(void) {
      Wire.begin();
      Wire.setClock(400000);
      
      Wire1.begin();
      Wire1.setClock(400000);
    
      _frontDistanceSensorMux = new VL6180I2CMux(&Wire1, 0x29, NUM_FRONT_DISTANCE_SENSORS,
        FRONT_MUX_PIN0,FRONT_MUX_PIN1,FRONT_MUX_PIN2);
      _frontDistanceSensorMux->initializeSensors();
      
      _rearDistanceSensorMux = new VL6180I2CMux(&Wire, 0x29, NUM_REAR_DISTANCE_SENSORS,
        REAR_MUX_PIN0,REAR_MUX_PIN1,REAR_MUX_PIN2);
      _rearDistanceSensorMux->initializeSensors();
    };

    void setDistanceThreshold(int threshold) {
      _distanceThreshold = threshold;
    };
    
    unsigned int getDistanceFromObject(Orientation orientation, SensorLocation location) {
      VL6180I2CMux* sensorMux = 
        (orientation == FRONT) ? _frontDistanceSensorMux : _rearDistanceSensorMux;
      return min(_distanceThreshold, sensorMux->readDistance(location));
    };
    
  private:
    // Distance sensors
    VL6180I2CMux* _frontDistanceSensorMux;
    VL6180I2CMux* _rearDistanceSensorMux;

    int _distanceThreshold;

};

#endif // DISTANCESENSORS_H
