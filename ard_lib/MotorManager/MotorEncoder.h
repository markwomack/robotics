//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef MotorEncoder_h
#define MotorEncoder_h

// Arduino includes
#include <inttypes.h>

/*
 * Defines the interface for a motor encoder.
 */
class MotorEncoder {
  public:
    MotorEncoder(){};

    /*
     * Returns the current value of the encoder.
     */
    virtual int32_t read(void) = 0;

    /*
     * Sets the current value of the encoder to the given value,
     * returns what the value of the encoder was before setting the
     * new value.
     */ 
    virtual int32_t write(int32_t value) = 0;

    /*
     * Returns the current value of the reported faults
     */
    virtual int32_t readFaults(void) = 0;
};
#endif // MotorEncoder_h