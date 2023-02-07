//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// This is an example of using the PololuQik2s9v1 motor
// controller with two motors, each with a quadrature motor
// encoder.

// Arduino includes
#include <Arduino.h>

// Dependent includes
#include <DebugMsgs.h>
#include <TaskManager.h>
#include <BlinkTask.h>

// MotorManager includes
#include <PololuQik2s9v1MotorManager.h>
#include <QuadratureMotorEncoder.h>

// Pin Definitions - Defines all of the pins used
const uint8_t POLOLU_QIK_TX_PIN        =  0; // tx Pololu qik
const uint8_t POLOLU_QIK_RX_PIN        =  1; // rx Pololu qik
const uint8_t POLOLU_QIK_RESET_PIN     =  3; // reset Pololu qik
const uint8_t ENCODER_R_PHASE_B_PIN    = 31; // right encoder phase b signal
const uint8_t ENCODER_R_PHASE_A_PIN    = 32; // right encoder phase a signal
const uint8_t BUTTON_PIN               = 33; // start/stop button
const uint8_t ENCODER_L_PHASE_B_PIN    = 34; // left encoder phase b signal
const uint8_t ENCODER_L_PHASE_A_PIN    = 35; // left encoder phase a signal

// Motor and encoder manager
MotorManager* motorManager;

BlinkTask idleTask;

class ExerciseMotorsTask : public Task {
  public:
    ExerciseMotorsTask(){};

    void start(void) {
      _leftMotorSpeed = 0;
      _rightMotorSpeed = 0;
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);
      motorManager->writeEncoder(LEFT_MOTOR, 0);
      motorManager->writeEncoder(RIGHT_MOTOR, 0);
      _leftDir = false; // forward
      _rightDir = true; // reverse
    };

    void update(void) {
      if (abs(_leftMotorSpeed) >= (double)1.0) {
        DebugMsgs.debug().println("Switching left direction");
        _leftDir = !_leftDir;
      }
      if (abs(_rightMotorSpeed) >= (double)1.0) {
        DebugMsgs.debug().println("Switching right direction");
        _rightDir = !_rightDir;
      }
      _leftMotorSpeed += _leftDir ? -0.1 : 0.1;
      _rightMotorSpeed += _rightDir ? -0.1 : 0.1;

      DebugMsgs.debug().print("Left speed: ").print(_leftMotorSpeed).print(", Right speed: ").println(_rightMotorSpeed);
      
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);
    };

    void stop(void) {
      _leftMotorSpeed = 0;
      _rightMotorSpeed = 0;
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);
    };

  private:
    double _leftMotorSpeed;
    bool _leftDir;
    double _rightMotorSpeed;
    bool _rightDir;
};
ExerciseMotorsTask exerciseMotorsTask;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);
  
  // Setup the motor manager
  motorManager = 
    new PololuQik2s9v1MotorManager(
      POLOLU_QIK_TX_PIN, POLOLU_QIK_RX_PIN, POLOLU_QIK_RESET_PIN);

  // Setup encoders on the motor manager
  QuadratureMotorEncoder* leftEncoder = 
    new QuadratureMotorEncoder(ENCODER_L_PHASE_A_PIN, ENCODER_L_PHASE_B_PIN);
  QuadratureMotorEncoder* rightEncoder = 
    new QuadratureMotorEncoder(ENCODER_R_PHASE_A_PIN, ENCODER_R_PHASE_B_PIN);
  motorManager->setupEncoders(leftEncoder, rightEncoder);

  taskManager.addTask(&exerciseMotorsTask, 250);
  
  taskManager.addIdleTask(&idleTask, 100);
  taskManager.addBlinkTask(500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  taskManager.update();
}
