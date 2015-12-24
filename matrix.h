#ifndef matrix_h
#define matrix_h

class Matrix {
  public:
    // constructor
    Matrix(int din, int cs, int clk);
    void putData(char addr, char data);
    void putChipData(int chip, char addr, char data);
    void putByte(char b);
    void setScanLimit(char b);
    void setDecodeMode(char b);
    void setShutdownMode(char b);
    void setTestMode(char b);
    void setIntensity(char b);
    // register bytes
    static const char reg_noop        = 0x00;
    static const char reg_dig0        = 0x01;
    static const char reg_dig1        = 0x02;
    static const char reg_dig2        = 0x03;
    static const char reg_dig3        = 0x04;
    static const char reg_dig4        = 0x05;
    static const char reg_dig5        = 0x06;
    static const char reg_dig6        = 0x07;
    static const char reg_dig7        = 0x08;
    static const char reg_decodeMode  = 0x09;
    static const char reg_intensity   = 0x0a;
    static const char reg_scanLimit   = 0x0b;
    static const char reg_shutdown    = 0x0c;
    static const char reg_test        = 0x0F;
  private:
    // data in
    int _din;
    // chip select
    int _cs;
    // clock
    int _clk;
};

#endif
