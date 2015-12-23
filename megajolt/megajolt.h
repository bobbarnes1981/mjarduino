#ifndef megajolt_h
#define megajolt_h

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

class Megajolt {
  public:
    // constructor
    Megajolt(HardwareSerial * serial);
    // function to retreive the current state from megajolt
    State getState();
    // request bytes
    static const char req_getState        = 0x53; // S
    static const char req_getVersion      = 0x75; // u
  private:
    // millis when request was sent
    unsigned long _sent_millis;
    // true if awaiting response, false if should send
    bool _sent;
    // serial object
    HardwareSerial * _serial;
};

#endif
