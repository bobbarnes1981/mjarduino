#include <Arduino.h>
#include "megajolt.h"

// constructor
Megajolt::Megajolt(HardwareSerial * serial) {
  _sent = false;
  _serial = serial;
}

// function to retreive the current state from megajolt
State Megajolt::getState() {
  // request has not been sent
  if (!_sent) {
    // send 'S' byte
    _serial->write(Megajolt::req_getState);
    // mark sent
    _sent = true;
    // record time
    _sent_millis = millis();
  }

  // default state object  
  State currentState = { false, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  // request has been sent
  if (_sent) {
    // all bytes available
    if (_serial->available() >= 9) {
      // set object as received
      currentState.received = true;

      // load all bytes
      currentState.advanceDegrees = _serial->read();
      currentState.rpmHi = _serial->read();
      currentState.rpmLo = _serial->read();
      currentState.bin = _serial->read(); // rpm (hi), load (lo)
      currentState.load = _serial->read();
      currentState.state = _serial->read(); // out1, out2, out3, out4, revLimit, shitfLight, reserved, config
      currentState.aux = _serial->read();
      currentState.advanceCorrectionBin = _serial->read();
      currentState.advanceCorrectionDeg = _serial->read();

      // mark as not sent
      _sent = false;
    } else if (millis() > _sent_millis + 10000) {
      // handle timeout
    }
  }

  // return result
  return currentState;
}
