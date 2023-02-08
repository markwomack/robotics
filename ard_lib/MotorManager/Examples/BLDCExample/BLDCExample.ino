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
#include <BLDCMotorManager.h>
#include <ThreePhaseMotorEncoder.h>

const uint8_t M1_DIR_PIN(2);
const uint8_t M1_BRAKE_PIN(3);
const uint8_t M1_SPEED_PIN(4);
const uint8_t M1_W_SIGNAL_PIN(5);  // green  - Hc
const uint8_t M1_V_SIGNAL_PIN(6);  // blue   - Hb
const uint8_t M1_U_SIGNAL_PIN(7);  // yellow - Ha
const uint8_t M2_W_SIGNAL_PIN(8);  // green  - Hc
const uint8_t M2_V_SIGNAL_PIN(9);  // blue   - Hb
const uint8_t M2_U_SIGNAL_PIN(10); // yellow - Ha
const uint8_t M2_DIR_PIN(11);
const uint8_t M2_BRAKE_PIN(12);
const uint8_t LED_BUILTIN_PIN(13); // Used by TaskManager blink task
const uint8_t M2_SPEED_PIN(14);
const uint8_t BUTTON_PIN(15);

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
      
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);

      DebugMsgs.debug().print("Left speed: ").print(_leftMotorSpeed).print(", Right speed: ").println(_rightMotorSpeed);
      DebugMsgs.debug().print("Left ticks: ").print(motorManager->readEncoder(LEFT_MOTOR)).print(", Right ticks: ").println(motorManager->readEncoder(RIGHT_MOTOR));
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
  motorManager = new BLDCMotorManager(M1_SPEED_PIN, M1_DIR_PIN, M1_BRAKE_PIN,
      M2_SPEED_PIN, M2_DIR_PIN, M2_BRAKE_PIN);

  // Setup encoders on the motor manager
  ThreePhaseMotorEncoder* leftEncoder = 
    new ThreePhaseMotorEncoder(M1_U_SIGNAL_PIN, M1_V_SIGNAL_PIN, M1_W_SIGNAL_PIN);
  ThreePhaseMotorEncoder* rightEncoder = 
    new ThreePhaseMotorEncoder(M2_U_SIGNAL_PIN, M2_V_SIGNAL_PIN, M2_W_SIGNAL_PIN);
  motorManager->setupEncoders(leftEncoder, rightEncoder);

  taskManager.addTask(&exerciseMotorsTask, 250);
  
  taskManager.addIdleTask(&idleTask, 100);
  taskManager.addBlinkTask(500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  taskManager.update();
}
