/*
 * Arduino program to read state from megajolt
 * and display to 7 segment display
 * 
 * -- PINS --
 * 
 * 0  - Serial RX
 * 1  - Serial TX
 * 
 * 2  - Max7219 DIN
 * 3  - Max7219 CS
 * 4  - Max7219 CLK
 * 
 * 5  - Button : switch between degrees and load
 *      (could we show AUX valule as well?)
 * 
 * 10 - LED On degrees / Off load
 * 11 - LED Shift light state
 * 12 - LED Rev limit state
 * 13 - On Board LED
 * 
 * Could use decimal points as shf/rev/ind leds
 * 
 */

#include "megajolt/megajolt.h"
#include "matrix/matrix.h"

// masks for state value
const byte MASK_SHF = 0x10; // bit 5
const byte MASK_REV = 0x08; // bit 4

// led pins
const int PIN_LED = 13;
const int PIN_REV = 12;
const int PIN_SHF = 11;
const int PIN_IND = 10;

// matrix pins
const int PIN_MATRIX_DIN = 2;
const int PIN_MATRIX_CS = 3;
const int PIN_MATRIX_CLK = 4;

// button pin
const int PIN_BTN = 5;

// indicate degrees or load display
bool showAdvanceDegrees = true;

Megajolt mj = Megajolt(&Serial);
Matrix m = Matrix(PIN_MATRIX_DIN, PIN_MATRIX_CS, PIN_MATRIX_CLK);

void setup() {
  // serial init
  Serial.begin(38400, SERIAL_8N1);

  // set pins for LEDs
  pinMode(PIN_IND, OUTPUT);
  pinMode(PIN_SHF, OUTPUT);
  pinMode(PIN_REV, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // set pin for button
  pinMode(PIN_BTN, INPUT);

  // matrix init
  m.setScanLimit(0x01);     // display 2 digits
  m.setDecodeMode(0xFF);    // BCD mode for all digits
  m.setShutdownMode(0x01);  // normal operation
  m.setTestMode(0x00);      // normal operation
  m.setIntensity(0x03);     // full brightness 0x0f

  // clear matrix
  for (int j = 1; j < 3; j++) {
    m.putData(j, 0x00);
  }
}

void loop() {  
  State currentState = mj.getState();

  if (currentState.received) {
    // on board led shows received
    digitalWrite(PIN_LED, LOW);

    // show shift light state
    digitalWrite(PIN_SHF, currentState.state & MASK_SHF);

    // show rev limit state
    digitalWrite(PIN_REV, currentState.state & MASK_REV);
    
    if (showAdvanceDegrees) {
      // show degrees
      displayBCDByte(currentState.advanceDegrees);
    } else {
      // show load
      displayBCDByte(currentState.load);
    }
  } else {
    // on board led shows waiting to receive
    digitalWrite(PIN_LED, HIGH); 
  }

  // indicate degrees or load
  digitalWrite(PIN_IND, showAdvanceDegrees);
  
  // todo: check button (with debounce)
}

void displayBCDByte(byte b) {
  // calculate tens and digits
  byte hi = b / 10;
  byte lo = b % 10;

  // write digits to display
  m.putChipData(0, 1, hi);
  m.putChipData(0, 2, lo);
}

