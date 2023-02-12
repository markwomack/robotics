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
    ThreePhaseMotorEncoder();
  
    void begin(const uint8_t phaseVPin, const uint8_t phaseWPin, const uint8_t phaseUPin);
    
    // MotorEncoder methods implemented
    int32_t read(void);
    int32_t write(int32_t value);
    int32_t readFaults(void);
    
  private:
    void handleInterrupt(const uint8_t pinNum, const uint8_t pinMask, const uint8_t pinShift);

    int32_t volatile _tickCount;   // Number of valid ticks counted
    int32_t volatile _faultCount;  // Number or encoder faults detected
    bool volatile _direction;      // The current direction of the motor
    uint8_t volatile _state;       // The current state as determined by the encoder phase pins
};

#endif // ThreePhaseMotorEncoder_h
