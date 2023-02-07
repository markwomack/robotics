//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

 #include "PololuQik2s9v1MotorManager.h"
  
PololuQik2s9v1MotorManager::PololuQik2s9v1MotorManager(
    uint8_t txPin, uint8_t rxPin, uint8_t resetPin) {
  qik = new PololuQik2s9v1(txPin, rxPin, resetPin);
  qik->init(9600);
}

int mapSpeed(double input)  {
  // Pin to value between -1 and 1, then multiply by 127
  // for expected qik value.
  return (int)(min(max(input, -1.0), 1.0) * 127);
}

void PololuQik2s9v1MotorManager::setMotorSpeed(Motor motor, double speed) {
  if (motor == LEFT_MOTOR) {
    qik->setM0Speed(mapSpeed(speed));
  } else {
    qik->setM1Speed(mapSpeed(speed));
  }
}

void PololuQik2s9v1MotorManager::setMotorSpeeds(double speedLeft, double speedRight) {
  qik->setSpeeds(mapSpeed(speedLeft), mapSpeed(speedRight));
}
