#ifndef megajolt_h
#define megajolt_h

enum request {
  req_none,
  req_getVersion,
  req_getState
};

// struct to represent the 'state' response from megajolt
struct State {
  // state has been received from megajolt
  bool received;
  
  // current ignition advance degrees
  byte advanceDegrees;
  // rpm hi byte
  byte rpmHi;
  // rpm low byte
  byte rpmLo;
  // current bin hi nibble rpm, lo nibble load
  byte bin;
  // current load value TPS/KPa
  byte load;
  // state out1, out2, out3, out4, revLimit, shitfLight, reserved, config
  byte state;
  // aux input value
  byte aux;
  // current advance correction bin
  byte advanceCorrectionBin;
  // current advance correction degrees
  byte advanceCorrectionDeg;
};

// struct to represent the 'version' response from megajolt
struct Version {
  // version has been received from megajolt
  bool received;
  
  // major version number
  byte major;
  // minor version number
  byte minor;
  // bugfix version number
  byte bugfix;
};

class Megajolt {
  public:
    // constructor
    Megajolt(HardwareSerial * serial);
    // function to retreive the current firmware version from megajolt
    Version getVersion();
    // function to retreive the current state from megajolt
    State getState();
    // request bytes
    static const char char_getState        = 0x53; // S
    static const char char_getVersion      = 0x56; // V
  private:
    // current request state
    request _request;
    // millis when request was sent
    unsigned long _sent_millis;
    // serial object
    HardwareSerial * _serial;
    // timeout value
    unsigned long _timeout;
};

#endif
