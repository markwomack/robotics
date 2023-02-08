//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// Arduino includes
#include <Arduino.h>

// Local includes
#include "ThreePhaseMotorEncoder.h"

const int MAX_ENCODERS(2);


struct EncoderInfo {
  int32_t volatile tickCount;   // Number of valid ticks counted
  char volatile prevPhase;      // Previous interrupt that was executed
};

EncoderInfo encoderInfo[MAX_ENCODERS];
int encoderCount = 0;

void U1Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_U_ENCODER_SIGNAL_PIN);
  
  encoderInfo[0].tickCount += (encoderInfo[0].prevPhase == 'W') ? 1 : -1;
  encoderInfo[0].prevPhase = 'U';
}

void V1Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_V_ENCODER_SIGNAL_PIN);
  
  encoderInfo[0].tickCount += (encoderInfo[0].prevPhase == 'U') ? 1 : -1;
  encoderInfo[0].prevPhase = 'V';
}

void W1Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_W_ENCODER_SIGNAL_PIN);

  encoderInfo[0].tickCount += (encoderInfo[0].prevPhase == 'V') ? 1 : -1;
  encoderInfo[0].prevPhase = 'W';
}

void U2Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_U_ENCODER_SIGNAL_PIN);
  
  encoderInfo[1].tickCount += (encoderInfo[1].prevPhase == 'W') ? 1 : -1;
  encoderInfo[1].prevPhase = 'U';
}

void V2Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_V_ENCODER_SIGNAL_PIN);
  
  encoderInfo[1].tickCount += (encoderInfo[1].prevPhase == 'U') ? 1 : -1;
  encoderInfo[1].prevPhase = 'V';
}

void W2Interrupt() {
  // Read the current signal value
  //int val = digitalRead(M1_W_ENCODER_SIGNAL_PIN);

  encoderInfo[1].tickCount += (encoderInfo[1].prevPhase == 'V') ? 1 : -1;
  encoderInfo[1].prevPhase = 'W';
}

ThreePhaseMotorEncoder::ThreePhaseMotorEncoder(uint8_t phaseUPin, uint8_t phaseVPin, uint8_t phaseWPin) {
  switch (encoderCount) {
  
    case 0:
      _encoderIndex = 0;
      
      pinMode(phaseUPin, INPUT_PULLUP);
      attachInterrupt(phaseUPin, U1Interrupt, CHANGE);
      
      pinMode(phaseVPin, INPUT_PULLUP);
      attachInterrupt(phaseVPin, V1Interrupt, CHANGE);
  
      pinMode(phaseWPin, INPUT_PULLUP);
      attachInterrupt(phaseWPin, W1Interrupt, CHANGE);
      
      encoderCount++;
      break;
      
    case 1:
      _encoderIndex = 1;
      
      pinMode(phaseUPin, INPUT_PULLUP);
      attachInterrupt(phaseUPin, U2Interrupt, CHANGE);
      
      pinMode(phaseVPin, INPUT_PULLUP);
      attachInterrupt(phaseVPin, V2Interrupt, CHANGE);
  
      pinMode(phaseWPin, INPUT_PULLUP);
      attachInterrupt(phaseWPin, W2Interrupt, CHANGE);
      
      encoderCount++;
      break;
      
    default:
      _encoderIndex = -1;
      break;
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
