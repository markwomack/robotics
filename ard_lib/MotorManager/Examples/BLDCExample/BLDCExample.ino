//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// This is an example of using a basic BLDC motor
// controller with two motors, each with a three phase motor
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

const float MAX_SPEED(0.1);

// Motor and encoder manager
MotorManager* motorManager;

BlinkTask idleTask;

class ExerciseMotorsTask : public Task {
  public:
    ExerciseMotorsTask(){};

    void start(void) {
      _leftMotorSpeed = 0;
      _rightMotorSpeed = 0;
      // Allow the motor to spin
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);
      motorManager->writeEncoder(LEFT_MOTOR, 0);
      motorManager->writeEncoder(RIGHT_MOTOR, 0);
      digitalWrite(M1_BRAKE_PIN, LOW);
      digitalWrite(M2_BRAKE_PIN, LOW);
      
      _leftDir = false; // forward
      _leftBraking = false;
      _leftLastEncoder = 0;
      _rightDir = true; // reverse
      _rightBraking = false;
      _rightLastEncoder = 0;
    };

    void update(void) {
      if (!_leftBraking) {
        if (abs(_leftMotorSpeed) >= MAX_SPEED) {
          DebugMsgs.debug().println("Braking left motor");
          motorManager->setMotorSpeed(LEFT_MOTOR, 0);
          digitalWrite(M1_BRAKE_PIN, HIGH);
          _leftBraking = true;
          _leftLastEncoder = motorManager->readEncoder(LEFT_MOTOR);
        } else {
          _leftMotorSpeed += _leftDir ? -0.1 : 0.1;
          motorManager->setMotorSpeed(LEFT_MOTOR, _leftMotorSpeed);
        }
      } else {
        int32_t curEncoder = motorManager->readEncoder(LEFT_MOTOR);
        if (curEncoder - _leftLastEncoder == 0) {
          DebugMsgs.debug().println("Switching direction left motor");
          digitalWrite(M1_BRAKE_PIN, LOW);
          _leftBraking = false;
          _leftDir = !_leftDir;
          _leftMotorSpeed = 0;
        }
        _leftLastEncoder = curEncoder;
      }
      
      if (!_rightBraking) {
        if (abs(_rightMotorSpeed) >= MAX_SPEED) {
          DebugMsgs.debug().println("Braking right motor");
          motorManager->setMotorSpeed(RIGHT_MOTOR, 0);
          digitalWrite(M2_BRAKE_PIN, HIGH);
          _rightBraking = true;
          _rightLastEncoder = motorManager->readEncoder(RIGHT_MOTOR);
        } else {
          _rightMotorSpeed += _rightDir ? -0.1 : 0.1;
          motorManager->setMotorSpeed(RIGHT_MOTOR, _rightMotorSpeed);
        }
      } else {
        int32_t curEncoder = motorManager->readEncoder(RIGHT_MOTOR);
        if (curEncoder - _rightLastEncoder == 0) {
          DebugMsgs.debug().println("Switching direction right motor");
          digitalWrite(M2_BRAKE_PIN, LOW);
          _rightBraking = false;
          _rightDir = !_rightDir;
          _rightMotorSpeed = 0;
        }
        _rightLastEncoder = curEncoder;
      }
      
      DebugMsgs.debug().print("Left speed: ").print(_leftMotorSpeed).print(", Right speed: ").println(_rightMotorSpeed);
      DebugMsgs.debug().print("Left ticks: ").print(motorManager->readEncoder(LEFT_MOTOR)).print(", Right ticks: ").println(motorManager->readEncoder(RIGHT_MOTOR));
      DebugMsgs.debug().print("Left faults: ").print(motorManager->readEncoderFaults(LEFT_MOTOR)).print(", Right faults: ").println(motorManager->readEncoderFaults(RIGHT_MOTOR));
    };

    void stop(void) {
      _leftMotorSpeed = 0;
      _rightMotorSpeed = 0;
      motorManager->setMotorSpeeds(_leftMotorSpeed, _rightMotorSpeed);
      
      digitalWrite(M1_BRAKE_PIN, HIGH);
      digitalWrite(M2_BRAKE_PIN, HIGH);
    };

  private:
    float _leftMotorSpeed;
    bool _leftDir;
    bool _leftBraking;
    int32_t _leftLastEncoder;
    float _rightMotorSpeed;
    bool _rightDir;
    bool _rightBraking;
    int32_t _rightLastEncoder;
};
ExerciseMotorsTask exerciseMotorsTask;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);
  
  // Setup the motor manager
  motorManager = new BLDCMotorManager(M1_SPEED_PIN, M1_DIR_PIN, M1_BRAKE_PIN,
      M2_SPEED_PIN, M2_DIR_PIN, M2_BRAKE_PIN);

  // Setup encoders on the motor manager
  ThreePhaseMotorEncoder* leftEncoder = new ThreePhaseMotorEncoder();
  leftEncoder->begin(M1_V_SIGNAL_PIN, M1_W_SIGNAL_PIN, M1_U_SIGNAL_PIN);
  ThreePhaseMotorEncoder* rightEncoder = new ThreePhaseMotorEncoder();
  rightEncoder->begin(M2_V_SIGNAL_PIN, M2_W_SIGNAL_PIN, M2_U_SIGNAL_PIN);
  motorManager->setupEncoders(leftEncoder, rightEncoder);

  taskManager.addTask(&exerciseMotorsTask, 500);
  
  taskManager.addIdleTask(&idleTask, 100);
  taskManager.addBlinkTask(500);
  taskManager.startMonitoringButton(BUTTON_PIN, HIGH);
}

void loop() {
  taskManager.update();
}
