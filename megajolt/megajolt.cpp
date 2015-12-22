#include <Arduino.h>
#include "megajolt.h"

Megajolt::Megajolt()
{
}

// function to retreive the current state from megajolt
State Megajolt::getState() {
  Serial.write(Megajolt::req_state);
  
  State currentState = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  while(Serial.available() < 9) {
    // spin
  }
  
  currentState.advanceDegrees = Serial.read();
  currentState.rpmHi = Serial.read();
  currentState.rpmLo = Serial.read();
  currentState.bin = Serial.read(); // rpm (hi), load (lo)
  currentState.load = Serial.read();
  currentState.state = Serial.read(); // out1, out2, out3, out4, revLimit, shitfLight, reserved, config
  currentState.aux = Serial.read();
  currentState.advanceCorrectionBin = Serial.read();
  currentState.advanceCorrectionDeg = Serial.read();
  
  return currentState;
}

