//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef EDGESENSORS_H
#define EDGESENSORS_H

// Third party includes
#include <QTRSensors.h>

// My includes
#include <DebugMsgs.h>

// Local includes
#include "pin_assignments.h"
#include "robot_constants.h"

class EdgeSensors {
  public:
    EdgeSensors() {};

    void initialize(void) {
      _sensors = new QTRSensors();
      _sensors->setTypeRC();
      _sensors->setSensorPins((const uint8_t[]){
        FL_EDGE_SENSOR_PIN, FR_EDGE_SENSOR_PIN,
        RL_EDGE_SENSOR_PIN, RR_EDGE_SENSOR_PIN},
        NUM_EDGE_SENSORS);
    };

    void read(void) {
      // read raw sensor values
      _sensors->read(_sensorValues);
    };

    uint16_t value(uint8_t sensor) {
      return _sensorValues[sensor];
    }

  private:
    QTRSensors* _sensors;
    uint16_t _sensorValues[NUM_EDGE_SENSORS];
};

#endif // EDGESENSORS_H
