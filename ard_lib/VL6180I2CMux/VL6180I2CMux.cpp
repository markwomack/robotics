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

#include "VL6180I2CMux.h"
#include <SerialDebug.h>

VL6180I2CMux::VL6180I2CMux(
  int sensorAddress, uint8_t numSensors, uint8_t muxPin0,
  uint8_t muxPin1, uint8_t muxPin2) {
    _numSensors = numSensors;
    _muxPin0 = muxPin0;
    _muxPin1 = muxPin1;
    _muxPin2 = muxPin2;
    _sensor = new VL6180X();
    _sensor->setAddress(sensorAddress);
    _calibrationOffsets = new int[_numSensors];
    for (int i = 0; i < _numSensors; i++) {
      _calibrationOffsets[i] = 0;
    }
  }
  
  VL6180I2CMux::VL6180I2CMux(
  TwoWire* i2cBus, int sensorAddress, uint8_t numSensors, uint8_t muxPin0,
  uint8_t muxPin1, uint8_t muxPin2) {
    _numSensors = numSensors;
    _muxPin0 = muxPin0;
    _muxPin1 = muxPin1;
    _muxPin2 = muxPin2;
    _sensor = new VL6180X();
    _sensor->setBus(i2cBus);
    _sensor->setAddress(sensorAddress);
    _calibrationOffsets = new int[_numSensors];
    for (int i = 0; i < _numSensors; i++) {
      _calibrationOffsets[i] = 0;
    }
  }

void VL6180I2CMux::selectSensor(uint8_t sensorNum) {
  digitalWrite(_muxPin0, LOW);
  digitalWrite(_muxPin1, LOW);
  digitalWrite(_muxPin2, LOW);
  
  if (B00000001 & sensorNum) {
    digitalWrite(_muxPin0, HIGH);
  }
  
  if (B00000010 & sensorNum) {
    digitalWrite(_muxPin1, HIGH);
  }

  if (B00000100 & sensorNum) {
    digitalWrite(_muxPin2, HIGH);
  }
}

int VL6180I2CMux::initializeSensors() {
  pinMode(_muxPin0, OUTPUT);
  pinMode(_muxPin1, OUTPUT);
  pinMode(_muxPin2, OUTPUT);
  int retVal = 0;
  for (uint8_t x = 0; x < _numSensors; x++) {
    selectSensor(x);
    _sensor->init();
    _sensor->configureDefault();
    _sensor->setTimeout(100);
  }
  return retVal;
}

void VL6180I2CMux::setCalibrationOffsets(const int* offsets) {
  memcpy(_calibrationOffsets, offsets, sizeof(int)*_numSensors);
}

void VL6180I2CMux::writeRegister(uint8_t sensorNum, uint16_t reg, uint8_t value) {
  selectSensor(sensorNum);
  _sensor->writeReg(reg, value);
}

void VL6180I2CMux::writeRegister16Bit(uint8_t sensorNum, uint16_t reg, uint16_t value) {
  selectSensor(sensorNum);
  _sensor->writeReg16Bit(reg, value);
}

void VL6180I2CMux::writeRegister32Bit(uint8_t sensorNum, uint16_t reg, uint32_t value) {
  selectSensor(sensorNum);
  _sensor->writeReg32Bit(reg, value);
}

uint8_t VL6180I2CMux::readRegister(uint8_t sensorNum, uint16_t reg) {
  selectSensor(sensorNum);
  return _sensor->readReg(reg);
}

uint16_t VL6180I2CMux::readRegister16Bit(uint8_t sensorNum, uint16_t reg) {
  selectSensor(sensorNum);
  return _sensor->readReg16Bit(reg);
}

uint32_t VL6180I2CMux::readRegister32Bit(uint8_t sensorNum, uint16_t reg) {
  selectSensor(sensorNum);
  return _sensor->readReg32Bit(reg);
}
    
uint16_t VL6180I2CMux::readDistance(uint8_t sensorNum) {
  if (sensorNum >= _numSensors) {
    return 0;
  }

  selectSensor(sensorNum);
  uint16_t distance = _sensor->readRangeSingle();
  if (_sensor->timeoutOccurred()) {
    return 0;
  } else {
    if ((distance + _calibrationOffsets[sensorNum]) < 0) {
      return 0;
    } else {
      return distance + _calibrationOffsets[sensorNum];
    }
  }
}
