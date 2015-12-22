#ifndef megajolt_h
#define megajolt_h

// struct to represent the 'state' response from megajolt
struct State {
  byte advanceDegrees;
  byte rpmHi;
  byte rpmLo;
  byte bin; // rpm (hi), load (lo)
  byte load;
  byte state; // out1, out2, out3, out4, revLimit, shitfLight, reserved, config
  byte aux;
  byte advanceCorrectionBin;
  byte advanceCorrectionDeg;
};

class Megajolt {
  public:
    Megajolt();
    State getState();
    static const char req_state        = 0x53; // S
};

#endif
