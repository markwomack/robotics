//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef ThreePhaseMotorEncoder_h
#define ThreePhaseMotorEncoder_h

#include <inttypes.h>

#include "MotorEncoder.h"

class ThreePhaseMotorEncoder : public MotorEncoder {
  public:
    ThreePhaseMotorEncoder(uint8_t phaseVPin, uint8_t phaseWPin, uint8_t phaseUPin);
  
    int32_t read(void);
    int32_t write(int32_t value);
    int32_t readFaults(void);
    
  private:
    void setEncoder(void);
    int _encoderIndex;
};

#endif // ThreePhaseMotorEncoder_h
