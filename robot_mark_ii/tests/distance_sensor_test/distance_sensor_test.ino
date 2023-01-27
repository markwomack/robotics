//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//
 
// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// Teensy includes
#include <WireIMXRT.h> // Teensy Wire replacement

// Project includes
#include <DebugMsgs.h>
#include <TaskManager.h>
#include <Task.h>
#include <VL6180I2CMux.h>

const uint8_t REAR_MUX_PIN2     =  4; // rear distance sensor mux selector pin 2
const uint8_t REAR_MUX_PIN1     =  5; // rear distance sensor mux selector pin 1
const uint8_t REAR_MUX_PIN0     =  6; // rear distance sensor mux selector pin 0
const uint8_t FRONT_MUX_PIN0    =  7; // front distance sensor mux selector pin 0
const uint8_t FRONT_MUX_PIN1    =  8; // front distance sensor mux selector pin 1
const uint8_t FRONT_MUX_PIN2    =  9; // front distance sensor mux selector pin 2
const uint8_t BUTTON_PIN        = 33; // start/stop button

VL6180I2CMux* frontSensorMux;
int numFrontMuxSensor = 5;

VL6180I2CMux* rearSensorMux;
int numRearMuxSensor = 5;

class ReadSensorsTask : public Task {
  public:
    ReadSensorsTask(){};

    void setup(void) {
      Wire.begin();
      Wire.setClock(400000);
      
      Wire1.begin();
      Wire1.setClock(400000);
    
      frontSensorMux = new VL6180I2CMux(&Wire1, 0x29, numFrontMuxSensor,
        FRONT_MUX_PIN0,FRONT_MUX_PIN1,FRONT_MUX_PIN2);
      frontSensorMux->initializeSensors();
      
      rearSensorMux = new VL6180I2CMux(&Wire, 0x29, numRearMuxSensor,
        REAR_MUX_PIN0,REAR_MUX_PIN1,REAR_MUX_PIN2);
      rearSensorMux->initializeSensors();

      DebugMsgs.debug().println("Sketch setup");
    };

    void update(void) {
      DebugMsgs.debug().print("front: ");
      for (uint8_t x = 0; x < numFrontMuxSensor; x++) {
        DebugMsgs.print(x).print(": ").print(frontSensorMux->readDistance(x)).print(", ");
      }
      DebugMsgs.println();
      DebugMsgs.debug().print("rear:  ");
      for (int8_t x = 0; x < numRearMuxSensor; x++) {
        DebugMsgs.print(x).print(": ").print(rearSensorMux->readDistance(x)).print(", ");
      }
      DebugMsgs.println().println();
    }
};
ReadSensorsTask readSensorsTask;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  taskManager.addTask(&readSensorsTask, 1000);
  taskManager.addBlinkTask(500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  // Allow the taskmanager to perform a loop call to check for button presses
  // and to execute the tasks
  taskManager.update();
}
