//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

#include <QTRSensors.h>

// Project includes
#include <DebugMsgs.h>
#include <TaskManager.h>
#include <Task.h>

const uint8_t RR_EDGE_SENSOR_PIN =  2; // rear-right edge sensor
const uint8_t RL_EDGE_SENSOR_PIN = 17; // rear-left edge sensor
const uint8_t FR_EDGE_SENSOR_PIN = 30; // front-right edge sensor
const uint8_t BUTTON_PIN         = 33; // start/stop button
const uint8_t FL_EDGE_SENSOR_PIN = 36; // front_left edge sensor

const uint8_t FL = 0;
const uint8_t FR = 1;
const uint8_t RL = 2;
const uint8_t RR = 3;

QTRSensors qtr;
const uint8_t SENSORCOUNT(4);
uint16_t sensorValues[SENSORCOUNT];

class ReadEdgeSensorsTask : public Task {
  public:
    ReadEdgeSensorsTask() {};

    void setup(void) {
      // configure the sensors
      qtr.setTypeRC();
      qtr.setSensorPins((const uint8_t[]){
        FL_EDGE_SENSOR_PIN, FR_EDGE_SENSOR_PIN,
        RL_EDGE_SENSOR_PIN, RR_EDGE_SENSOR_PIN}, SENSORCOUNT);
    };

    void update(void) {
      // read raw sensor values
      qtr.read(sensorValues);
    
      DebugMsgs.debug().print("FL: ").print(sensorValues[FL] > 200 ? "EDGE" : "TBLE").print("  ");
      DebugMsgs.print("FR: ").println(sensorValues[FR] > 200 ? "EDGE" : "TBLE");
      DebugMsgs.debug().print("RL: ").print(sensorValues[RL] > 200 ? "EDGE" : "TBLE").print("  ");
      DebugMsgs.print("RR: ").println(sensorValues[RR] > 200 ? "EDGE" : "TBLE");
      DebugMsgs.println();
    };
};
ReadEdgeSensorsTask readEdgeSensorsTask;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  taskManager.addBlinkTask(500);
  taskManager.addTask(&readEdgeSensorsTask, 500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  // Execute the tasks
  taskManager.update();
}
