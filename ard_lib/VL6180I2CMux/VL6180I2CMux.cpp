//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include "VL6180I2CMux.h"

VL6180I2CMux::VL6180I2CMux(
  int sensorAddress, uint8_t numSensors, uint8_t muxPin0,
  uint8_t muxPin1, uint8_t muxPin2) {
    _numSensors = numSensors;
    _muxPin0 = muxPin0;
    _muxPin1 = muxPin1;
    _muxPin2 = muxPin2;
    _sensor = new VL6180X();
    _sensor->setAddress(sensorAddress);
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
    // slight pause to make sure the select is complete
    delay(100);
    _sensor->init();
    _sensor->configureDefault();
    _sensor->setTimeout(100);
  }
  return retVal;
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
    return distance;
  }
}
