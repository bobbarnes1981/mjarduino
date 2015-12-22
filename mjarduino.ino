/*
 * Arduino program to read state from megajolt
 * and display to 7 segment display
 */

#include "megajolt/megajolt.h"
#include "matrix/matrix.h"

const int LED_PIN = 13;

const int DIN_PIN = 3;
const int CS_PIN = 4;
const int CLK_PIN = 5;

Megajolt mj = Megajolt();
Matrix m = Matrix(DIN_PIN, CS_PIN, CLK_PIN);

void setup() {
  Serial.begin(38400, SERIAL_8N1);
  
  pinMode(LED_PIN, OUTPUT);

  // matrix init
  m.setScanLimit(0x02);     // display 2 digits
  m.setDecodeMode(0xFF);    // BCD mode for all digits
  m.setShutdownMode(0x01);  // normal operation
  m.setTestMode(0x00);      // normal operation
  m.setIntensity(0x03);     // full brightness 0x0f
  for (int j = 1; j < 3; j++)
  {
    m.putData(j, 0x00);
  }
}

void loop() {  
  State currentState = mj.getState();

  if (currentState.received) {
    digitalWrite(LED_PIN, LOW);
    
    // calculate tens and digits
    byte adv = currentState.advanceDegrees;
    byte advHi = adv / 10;
    byte advLo = adv % 10;

    // write advance digits to display
    m.putByte(advHi);
    m.putByte(advLo);
  } else {
    digitalWrite(LED_PIN, HIGH); 
  }
}

