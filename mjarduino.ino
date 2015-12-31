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
 * 5  - Button : switch between degrees, load and aux
 * 
 * 11 - LED Shift light state
 * 12 - LED Rev limit state
 * 13 - On Board LED
 * 
 * Could use decimal points as shf/rev leds
 * 
 */

//#define DEBUG

#include "mjarduino.h"

#include "button.h"
#include "matrix.h"
#include "megajolt.h"

// masks for state value
const byte MASK_OUT1 = 0x01; // bit 0
const byte MASK_OUT2 = 0x02; // bit 1
const byte MASK_OUT3 = 0x04; // bit 2
const byte MASK_OUT4 = 0x08; // bit 3
const byte MASK_REV =  0x10; // bit 4
const byte MASK_SHF =  0x20; // bit 5
const byte MASK_RES =  0x40; // bit 6
const byte MASK_CFG =  0x80; // bit 7

// led pins
const int PIN_LED = 13;
const int PIN_REV = 12;
const int PIN_SHF = 11;

// matrix pins
const int PIN_MATRIX_DIN = 2;
const int PIN_MATRIX_CS = 3;
const int PIN_MATRIX_CLK = 4;

// button pin
const int PIN_BTN = 5;

// indicate degrees, load display or aux
displaystate state = state_degrees;

// last button state
bool buttonState = false;

// objects
Button b = Button(PIN_BTN);
Matrix m = Matrix(PIN_MATRIX_DIN, PIN_MATRIX_CS, PIN_MATRIX_CLK);
Megajolt mj = Megajolt(&Serial);

// megajolt firmware version
Version v;

void setup() {
  // serial init
  Serial.begin(38400, SERIAL_8N1);

  // set pins for LEDs
  pinMode(PIN_SHF, OUTPUT);
  pinMode(PIN_REV, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // matrix init
  m.setScanLimit(0x01);     // display 2 digits
  m.setDecodeMode(0xFF);    // BCD mode for all digits
  m.setShutdownMode(0x01);  // normal operation
  m.setTestMode(0x00);      // normal operation
  m.setIntensity(0x01);     // full brightness 0x0f

  // clear matrix
  for (int j = 1; j < 3; j++) {
    m.putData(j, 0x00);
  }

  do {
    // get the version
    v = mj.getVersion();
  } while(v.received == false);

  #ifdef DEBUG
  Serial.write("version:");
  Serial.write(v.major);
  Serial.write(v.minor);
  Serial.write(v.bugfix);
  #endif
}

void loop() {
  // get the state
  State currentState = mj.getState();

  if (currentState.received) {
    // on board led shows received
    digitalWrite(PIN_LED, LOW);

    // show shift light state
    digitalWrite(PIN_SHF, currentState.state & MASK_SHF);

    // show rev limit state
    digitalWrite(PIN_REV, currentState.state & MASK_REV);

    // show selected info on display
    switch(state) {
      case state_degrees:
        displayBCDByte(currentState.advanceDegrees);
        break;
      case state_load:
        displayBCDByte(currentState.load);
        break;
      case state_aux:
        displayBCDByte(currentState.aux);
        break;
    }
  } else {
    // on board led shows waiting to receive
    digitalWrite(PIN_LED, HIGH); 
  }
      
  // if button state has changed
  if (buttonState != b.getState()) {
    // update current state
    buttonState = !buttonState;
    // if button was pressed
    if (buttonState) {
      // cycle display state
      switch(state) {
        case state_degrees:
          state = state_load;
          break;
        case state_load:
          state = state_aux;
          break;
        case state_aux:
          state = state_degrees;
          break;
      }
    
      #ifdef DEBUG
      Serial.write("state:");
      Serial.write(state);
      #endif
    }
  }
}

void displayBCDByte(byte b) {
  // calculate hundreds
  byte h = b / 100;
  
  // calculate tens
  byte t = (b - (h * 100)) / 10;

  // calculate units
  byte u = b - (h * 100) - (t * 10);
  
  // write digits to display
  //m.putChipData(0, , h);
  m.putChipData(0, 1, t);
  m.putChipData(0, 2, u);

  #ifdef DEBUG
  Serial.write("BCD:");
  Serial.write(h+48);
  Serial.write(t+48);
  Serial.write(u+48);
  #endif
}

