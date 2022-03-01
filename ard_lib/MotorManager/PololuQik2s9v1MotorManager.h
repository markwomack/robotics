//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PololuQik2s9v1MotorManager_h
#define PololuQik2s9v1MotorManager_h

#include <inttypes.h>
#include <PololuQik.h>
#include "MotorManager.h"

class PololuQik2s9v1MotorManager : public MotorManager
{

public:
  PololuQik2s9v1MotorManager(uint8_t txPin, uint8_t rxPin, uint8_t resetPin);

  void setMotorSpeed(Motor motor, double speed);
  void setMotorSpeeds(double speedLeft, double speedRight);
  
private:
  PololuQik2s9v1* qik;
};

#endif
