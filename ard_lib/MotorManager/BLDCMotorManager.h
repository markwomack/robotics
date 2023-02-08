//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef BLDCMotorManager_h
#define BLDCMotorManager_h

// Arduino includes
#include <inttypes.h>

// Local includes
#include "MotorManager.h"

class BLDCMotorManager : public MotorManager {

public:
  BLDCMotorManager(uint8_t m1SpeedPin, uint8_t m1DirPin, uint8_t m1BrakePin,
      uint8_t m2SpeedPin, uint8_t m2DirPin, uint8_t m2BrakePin);

  void setMotorSpeed(Motor motor, double speed);
  void setMotorSpeeds(double speedLeft, double speedRight);
  
private:
  uint8_t _m1SpeedPin;
  uint8_t _m1DirPin;
  uint8_t _m1BrakePin;
  uint8_t _m2SpeedPin;
  uint8_t _m2DirPin;
  uint8_t _m2BrakePin;
  
  int mapSpeed(double input);
};

#endif // BLDCMotorManager_h
