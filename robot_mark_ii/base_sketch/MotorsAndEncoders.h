//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef MOTORSANDENCODER_H
#define MOTORSANDENCODER_H

// My includes
#include <PololuQik2s9v1MotorManager.h>
#include <QuadratureMotorEncoder.h>
#include <MotorController.h>

// Local includes
#include "pin_assignments.h"
#include "robot_constants.h"

class MotorsAndEncoders {
  public:
    MotorsAndEncoders() {};

    void initialize(void) {
      // Setup the motor manager
      _motorManager = 
        new PololuQik2s9v1MotorManager(
          POLOLU_QIK_TX_PIN, POLOLU_QIK_RX_PIN, POLOLU_QIK_RESET_PIN);
    
      // Setup encoders on the motor manager
      QuadratureMotorEncoder* leftEncoder = 
        new QuadratureMotorEncoder(ENCODER_L_PHASE_A_PIN, ENCODER_L_PHASE_B_PIN);
      QuadratureMotorEncoder* rightEncoder = 
        new QuadratureMotorEncoder(ENCODER_R_PHASE_A_PIN, ENCODER_R_PHASE_B_PIN);
      _motorManager->setupEncoders(leftEncoder, rightEncoder);
    
      // Setup the motor controller
      _motorController = new MotorController(_motorManager,
         KP, KI, KD, 50, RADIANS_PER_TICK, MAX_RADIANS_PER_SECOND);
    };

    void stop() {
      _motorController->stop();
      _motorManager->setMotorSpeeds(0, 0);
    };
    
    void stopAndResetEncoders() {
      _motorController->stop();
      _motorManager->setMotorSpeeds(0, 0);
      _motorManager->readAndResetEncoder(LEFT_MOTOR);
      _motorManager->readAndResetEncoder(RIGHT_MOTOR);
    };

    int32_t readEncoder(Motor motor) {
      return _motorManager->readEncoder(motor);
    };
    
    void resetEncoders() {
      _motorManager->readAndResetEncoder(LEFT_MOTOR);
      _motorManager->readAndResetEncoder(RIGHT_MOTOR);
    }
      
    void setMotorSpeeds(double speedLeft, double speedRight) {
      _motorController->stop();
      _motorManager->setMotorSpeeds(speedLeft, speedRight);
    };
      
    void setTargetSpeeds(double speedLeft, double speedRight) {
      _motorController->start();
      _motorController->setDesiredSpeeds(speedLeft, speedRight);
    };

    void adjustSpeeds(void) {
      _motorController->adjustSpeeds();
    };

  private:
    // Motor and encoder manager
    MotorManager* _motorManager;
  
    // Motor controller
    MotorController* _motorController;
};

#endif // MOTORSANDENCODER_H
