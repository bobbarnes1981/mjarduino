#include <Arduino.h>
#include "megajolt.h"

Megajolt::Megajolt()
{
  _sent = false;
}

// function to retreive the current state from megajolt
State Megajolt::getState() {
  if (!_sent) {
    Serial.write(Megajolt::req_state);
    _sent = true;
    _sent_millis = millis();
  }
  
  State currentState = { false, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  if (_sent) {
    if (Serial.available() >= 9) {
      currentState.received = true;
      currentState.advanceDegrees = Serial.read();
      currentState.rpmHi = Serial.read();
      currentState.rpmLo = Serial.read();
      currentState.bin = Serial.read(); // rpm (hi), load (lo)
      currentState.load = Serial.read();
      currentState.state = Serial.read(); // out1, out2, out3, out4, revLimit, shitfLight, reserved, config
      currentState.aux = Serial.read();
      currentState.advanceCorrectionBin = Serial.read();
      currentState.advanceCorrectionDeg = Serial.read();

      _sent = false;
    } else if (millis() > _sent_millis + 10000) {
      // handle timeout
    }
  }
  
  return currentState;
}
