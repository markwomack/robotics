//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Base Arduino includes
#include <Arduino.h>
#include <inttypes.h>

// Project includes
#include <DebugMsgs.h>
#include <TaskManager.h>
#include <Task.h>
#include <PololuQik2s9v1MotorManager.h>

// Pin Definitions - Defines all of the pins used
const uint8_t POLOLU_QIK_TX_PIN      =  0; // tx Pololu qik
const uint8_t POLOLU_QIK_RX_PIN      =  1; // rx Pololu qik
const uint8_t POLOLU_QIK_RESET_PIN   =  3; // reset Pololu qik
const uint8_t ENCODER_RT_PHASE_B_PIN = 31; // right encoder phase b signal
const uint8_t ENCODER_RT_PHASE_A_PIN = 32; // right encoder phase a signal
const uint8_t BUTTON_PIN             = 33; // start/stop button
const uint8_t ENCODER_LT_PHASE_B_PIN = 34; // left encoder phase b signal
const uint8_t ENCODER_LT_PHASE_A_PIN = 35; // left encoder phase a signal

// Create the motor manager
MotorManager* motorManager;

class ApplyMotorStateTask : public Task {
  public:
    ApplyMotorStateTask() {};
    
    void start(void) {
      _direction = false;
      update();
    };

    void update(void) {
      // change direction
      DebugMsgs.debug().println("Changing direction!");
      bool direction = !_direction;
      double motorSpeed = (direction ? .75 : -.75);
      motorManager->setMotorSpeed(LEFT_MOTOR, motorSpeed);
      motorManager->setMotorSpeed(RIGHT_MOTOR, -motorSpeed);
      
      // store state for next update
      _direction = direction;
    };

    void stop(void) {
      motorManager->setMotorSpeeds(0,0);
    };
    
  private:
    bool _direction;
};
ApplyMotorStateTask applyMotorStateTask;

class ReadEncodersTask : public Task {
  public:
    ReadEncodersTask() {};

    void start(void) {
      // Reset the encoders
      motorManager->readAndResetEncoder(LEFT_MOTOR);
      motorManager->readAndResetEncoder(RIGHT_MOTOR);
    };
    
    void update(void) {
      DebugMsgs.debug().print("rightEncoder: ").println(motorManager->readEncoder(RIGHT_MOTOR));
      DebugMsgs.debug().print("leftEncoder:  ").println(motorManager->readEncoder(LEFT_MOTOR));
    };
};
ReadEncodersTask readEncodersTask;

void setup() {
	Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  // Setup the motor manager
  motorManager = 
    new PololuQik2s9v1MotorManager(
      POLOLU_QIK_TX_PIN, POLOLU_QIK_RX_PIN, POLOLU_QIK_RESET_PIN);
    
  // Setup encoders on the motor manager
  motorManager->setupEncoders(
    ENCODER_LT_PHASE_A_PIN, ENCODER_LT_PHASE_B_PIN,
    ENCODER_RT_PHASE_A_PIN, ENCODER_RT_PHASE_B_PIN);

  taskManager.addBlinkTask(500);
  taskManager.addTask(&applyMotorStateTask, 5000);
  taskManager.addTask(&readEncodersTask, 100);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
	// Execute the tasks
	taskManager.update();
}
