//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// V W U
// 0 0 1 = 1  <-these values are populated in the CWNEXTSTATE below
// 0 1 1 = 3    and CCWNEXTSTATE arrays below and are the only valid
// 0 1 0 = 2    transitions allowed eg from 6 you can only go to 2
// 1 1 0 = 6    (CCW)or 4 (CW).
// 1 0 0 = 4
// 1 0 1 = 5

// Arduino includes
#include <Arduino.h>

// Local includes
#include "ThreePhaseMotorEncoder.h"

const int MAX_ENCODERS(2);
const uint8_t VMASK = 0b00000011;
const uint8_t WMASK = 0b00000101;
const uint8_t UMASK = 0b00000110;
uint8_t CWNEXTSTATE[]  {0, 3, 6, 2, 5, 1, 4, 0 };
uint8_t CCWNEXTSTATE[] {0, 5, 3, 1, 6, 4, 2, 0 };
const bool CW(false);
const bool CCW(true);

struct EncoderInfo {
  int32_t volatile tickCount;   // Number of valid ticks counted
  int32_t volatile faultCount;
  bool volatile direction;
  uint8_t vPin;
  uint8_t wPin;
  uint8_t uPin;
  uint8_t volatile state;
};

EncoderInfo encoderInfo[MAX_ENCODERS];
int encoderCount = 0;

void handleInterrupt(const uint8_t encoderIndex, const uint8_t newState) {
  // If the new state is in the CW array, then going CW
  if (newState == CWNEXTSTATE[encoderInfo[encoderIndex].state]) {
    encoderInfo[encoderIndex].tickCount++;
    encoderInfo[encoderIndex].direction = CW;
    encoderInfo[encoderIndex].state = newState;
  // If the new state is in the CCW array, then going CCW
  } else if (newState == CCWNEXTSTATE[encoderInfo[encoderIndex].state]) {
    encoderInfo[encoderIndex].tickCount--;
    encoderInfo[encoderIndex].direction = CCW;
    encoderInfo[encoderIndex].state = newState;
  // This is noise to be ignored, but recorded
  } else {
    encoderInfo[encoderIndex].faultCount++;
  }
}

void V0Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[0].vPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[0].state & VMASK) + (pinValue << 2);
  
  // handle the interrupt
  handleInterrupt(0, newState);
}

void W0Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[0].wPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[0].state & WMASK) + (pinValue << 1);
  
  // handle the interrupt
  handleInterrupt(0, newState);
}

void U0Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[0].uPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[0].state & UMASK) + pinValue;
  
  // handle the interrupt
  handleInterrupt(0, newState);
}

void V1Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[1].vPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[1].state & VMASK) + (pinValue << 2);
  
  // handle the interrupt
  handleInterrupt(1, newState);
}

void W1Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[1].wPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[1].state & WMASK) + (pinValue << 1);
  
  // handle the interrupt
  handleInterrupt(1, newState);
}

void U1Interrupt() {
  // Immediately capture the pin state
  uint8_t pinValue = digitalRead(encoderInfo[1].uPin);
  
  // Calculate the new state
  uint8_t newState = (encoderInfo[1].state & UMASK) + pinValue;
  
  // handle the interrupt
  handleInterrupt(1, newState);
}

ThreePhaseMotorEncoder::ThreePhaseMotorEncoder(uint8_t phaseVPin, uint8_t phaseWPin, uint8_t phaseUPin) {
  
  setEncoder();
  
  if (_encoderIndex > -1) {
    encoderInfo[_encoderIndex].vPin = phaseVPin;
    encoderInfo[_encoderIndex].wPin = phaseWPin;
    encoderInfo[_encoderIndex].uPin = phaseUPin;
    pinMode(phaseVPin, INPUT_PULLUP);
    pinMode(phaseWPin, INPUT_PULLUP);
    pinMode(phaseUPin, INPUT_PULLUP);
  
    uint8_t vVal = digitalRead(phaseVPin);
    uint8_t wVal = digitalRead(phaseWPin);
    uint8_t uVal = digitalRead(phaseUPin);
    encoderInfo[_encoderIndex].state = (vVal<<2) + (wVal<<1) + uVal;
  
    switch (_encoderIndex) {
  
      case 0:
        attachInterrupt(phaseUPin, U0Interrupt, CHANGE);
        attachInterrupt(phaseVPin, V0Interrupt, CHANGE);
        attachInterrupt(phaseWPin, W0Interrupt, CHANGE);
        break;
      
      case 1:
        attachInterrupt(phaseUPin, U1Interrupt, CHANGE);
        attachInterrupt(phaseVPin, V1Interrupt, CHANGE);
        attachInterrupt(phaseWPin, W1Interrupt, CHANGE);
        break;
      
      default:
        break;
    }
  }
}

void ThreePhaseMotorEncoder::setEncoder(void) {
  if (encoderCount < MAX_ENCODERS) {
    _encoderIndex = encoderCount++;
  } else {
    _encoderIndex = -1;
  }
}

int32_t ThreePhaseMotorEncoder::read(void) {
  if (_encoderIndex == -1) {
    return 0;
  }
  
  int32_t curValue;
  
  noInterrupts();
  curValue = encoderInfo[_encoderIndex].tickCount;
  interrupts();

  return curValue;
}

int32_t ThreePhaseMotorEncoder::write(int32_t value) {
  if (_encoderIndex == -1) {
    return 0;
  }
  
  int32_t curValue;
  
  noInterrupts();
  curValue = encoderInfo[_encoderIndex].tickCount;
  encoderInfo[_encoderIndex].tickCount = value;
  interrupts();
  
  return curValue;
}

int32_t ThreePhaseMotorEncoder::readFaults(void) {
  if (_encoderIndex == -1) {
    return 0;
  }
  
  int32_t curValue;
  
  noInterrupts();
  curValue = encoderInfo[_encoderIndex].faultCount;
  interrupts();

  return curValue;
}
