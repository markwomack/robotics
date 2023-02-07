//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef QuadratureMotorEncoder_h
#define QuadratureMotorEncoder_h

// Arduino includes
#include <inttypes.h>

// Dependent includes
#include <Encoder.h>

// Local includes
#include "MotorEncoder.h"

class QuadratureMotorEncoder : public MotorEncoder {
  public:
    QuadratureMotorEncoder(int8_t phaseAPin, int8_t phaseBPin);

    int32_t read(void);
    int32_t write(int32_t value);

  private:
    Encoder* _encoder;
};

#endif // QuadratureMotorEncoder_h

