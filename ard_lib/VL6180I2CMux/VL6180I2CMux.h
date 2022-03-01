//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef VL6180I2CMux_h
#define VL6180I2CMux_h

#include <inttypes.h>
#include <Wire.h>
#include <VL6180X.h>

class VL6180I2CMux
{
  public:
    VL6180I2CMux(int sensorAddress, uint8_t numSensors, uint8_t muxPin0,
      uint8_t muxPin1, uint8_t muxPin2);
    VL6180I2CMux(TwoWire* i2cBus, int sensorAddress, uint8_t numSensors,
      uint8_t muxPin0, uint8_t muxPin1, uint8_t muxPin2);
    int initializeSensors();
    
    void writeRegister(uint8_t sensorNum, uint16_t reg, uint8_t value);
    void writeRegister16Bit(uint8_t sensorNum, uint16_t reg, uint16_t value);
    void writeRegister32Bit(uint8_t sensorNum, uint16_t reg, uint32_t value);
    uint8_t readRegister(uint8_t sensorNum, uint16_t reg);
    uint16_t readRegister16Bit(uint8_t sensorNum, uint16_t reg);
    uint32_t readRegister32Bit(uint8_t sensorNum, uint16_t reg);
    
    uint16_t readDistance(uint8_t sensorNum);
    
  private:
    uint8_t _muxPin0;
    uint8_t _muxPin1;
    uint8_t _muxPin2;
    uint8_t _numSensors;
    VL6180X* _sensor;

    void selectSensor(uint8_t sensorNum);
};

#endif // VL6180I2CMux_h
