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

const uint8_t FL_SURFACE_SENSOR_PIN    = 20; // front-left surface sensor
const uint8_t FR_SURFACE_SENSOR_PIN    = 21; // front-right surface sensor
const uint8_t RR_SURFACE_SENSOR_PIN    = 22; // rear-right surface sensor
const uint8_t RL_SURFACE_SENSOR_PIN    = 23; // rear-left surface sensor
const uint8_t CTR_SURFACE_SENSOR_6_PIN = 24; // center surface sensor 6
const uint8_t CTR_SURFACE_SENSOR_5_PIN = 25; // center surface sensor 5
const uint8_t CTR_SURFACE_SENSOR_4_PIN = 26; // center surface sensor 4
const uint8_t CTR_SURFACE_SENSOR_3_PIN = 27; // center surface sensor 3
const uint8_t CTR_SURFACE_SENSOR_2_PIN = 28; // center surface sensor 2
const uint8_t CTR_SURFACE_SENSOR_1_PIN = 29; // center surface sensor 1
const uint8_t BUTTON_PIN               = 33; // start/stop button

QTRSensors qtr;
const uint8_t SENSORCOUNT(10);
uint16_t sensorValues[SENSORCOUNT];

class ReadSensorsTask : public Task {
  public:
    ReadSensorsTask() {};

    void start(void) {
      // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
      // = ~25 ms per calibrate() call.
      // Call calibrate() 400 times to make calibration take about 10 seconds.

      DebugMsgs.debug().println("Calibrating for light color");
      for (uint16_t i = 0; i < 200; i++){
        if (i % 2 == 0) {
          DebugMsgs.print('.');
        }
        qtr.calibrate();
      }
      DebugMsgs.println();
      DebugMsgs.debug().println("Calibrating for dark color");
      for (uint16_t i = 0; i < 200; i++)
      {
        if (i % 2 == 0) {
          DebugMsgs.print('.');
        }
        qtr.calibrate();
      }
      DebugMsgs.println();

      // print the calibration minimum values measured when emitters were on
      DebugMsgs.debug().println("****** MINIMUM VALUES");
      for (uint8_t i = 0; i < SENSORCOUNT; i++) {
        DebugMsgs.print(qtr.calibrationOn.minimum[i]).print(' ');
      }
      DebugMsgs.println();
    
      // print the calibration maximum values measured when emitters were on
      DebugMsgs.debug().println("****** MAXIMUM VALUES");
      for (uint8_t i = 0; i < SENSORCOUNT; i++) {
        DebugMsgs.print(qtr.calibrationOn.maximum[i]).print(' ');
      }
      DebugMsgs.println();
    };

    void update(void) {
      
      // read calibrated sensor values and obtain a measure of the line position
      // from 0 to 5000 (for a white line, use readLineWhite() instead)
      //uint16_t position = qtr.readLineBlack(sensorValues);
      
      // read raw sensor values
      qtr.read(sensorValues);
      
      // print the sensor values as numbers from 0 to 1000, where 0 means maximum
      // reflectance and 1000 means minimum reflectance, followed by the line
      // position
      DebugMsgs.debug()
        .print(sensorValues[0]).print('\t')
        .print(sensorValues[1]).println();
      DebugMsgs.debug()
        .print(sensorValues[2]).print('\t')
        .print(sensorValues[3]).print('\t')
        .print(sensorValues[4]).print('\t')
        .print(sensorValues[5]).print('\t')
        .print(sensorValues[6]).print('\t')
        .print(sensorValues[7]).println();
      DebugMsgs.debug()
        .print(sensorValues[8]).print('\t')
        .print(sensorValues[9]).println();
      DebugMsgs.println();
    };
    
};
ReadSensorsTask readSensorsTask;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){
    FL_SURFACE_SENSOR_PIN, FR_SURFACE_SENSOR_PIN,
    CTR_SURFACE_SENSOR_1_PIN, CTR_SURFACE_SENSOR_2_PIN, CTR_SURFACE_SENSOR_3_PIN,
    CTR_SURFACE_SENSOR_4_PIN, CTR_SURFACE_SENSOR_5_PIN, CTR_SURFACE_SENSOR_6_PIN,
    RL_SURFACE_SENSOR_PIN, RR_SURFACE_SENSOR_PIN
    }, SENSORCOUNT);

  taskManager.addBlinkTask(500);
  taskManager.addTask(&readSensorsTask, 500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  // Execute tasks
  taskManager.update();
}
