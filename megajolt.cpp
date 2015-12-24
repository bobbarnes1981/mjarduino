#include <Arduino.h>
#include "megajolt.h"

// constructor
Megajolt::Megajolt(HardwareSerial * serial) {
  _request = req_none;
  _serial = serial;
  _timeout = 1000;
}

// function to retreive the current firmware version from megajolt
Version Megajolt::getVersion() {
  // request has not been sent
  if (_request == req_none) {
    // send 'V' byte
    _serial->write(Megajolt::char_getVersion);
    // mark sent
    _request = req_getVersion;
    // record time
    _sent_millis = millis();
  }

  // default state object  
  Version currentVersion = { false, 0x00, 0x00, 0x00 };

  // request has been sent
  if (_request == req_getVersion) {
    // all bytes available
    if (_serial->available() >= 3) {
      // set object as received
      currentVersion.received = true;

      // load all bytes
      currentVersion.major = _serial->read();
      currentVersion.minor = _serial->read();
      currentVersion.bugfix = _serial->read();

      // mark as not sent
      _request = req_none;
    } else if (millis() > _sent_millis + _timeout) {
      // handle timeout
    }
  }

  // return result
  return currentVersion;
}

// function to retreive the current state from megajolt
State Megajolt::getState() {
  // request has not been sent
  if (_request == req_none) {
    // send 'S' byte
    _serial->write(Megajolt::char_getState);
    // mark sent
    _request = req_getState;
    // record time
    _sent_millis = millis();
  }

  // default state object  
  State currentState = { false, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  // request has been sent
  if (_request == req_getState) {
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
      _request = req_none;
    } else if (millis() > _sent_millis + _timeout) {
      // handle timeout
    }
  }

  // return result
  return currentState;
}
