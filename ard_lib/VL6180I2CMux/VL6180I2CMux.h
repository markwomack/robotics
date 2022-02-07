/*
 *      Please see the README.txt for a full description of this program and
 *      project.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

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
