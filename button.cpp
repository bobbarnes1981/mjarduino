#include <Arduino.h>
#include "button.h"

// constructor
Button::Button(int pin) {
  _pin = pin;
  _state = false;
  _delay = 50;
  _lastChange = 0;
  pinMode(_pin, INPUT);
}

// get debounced button state
bool Button::getState() {
  // get current button state
  int currentState = digitalRead(_pin);
  
  // check if current state is different to previous state
  if (currentState != _state) {
    // markt he time the state changed
    _lastChange = millis();
  }
  
  // if the button has been changed for more than 'delay'
  if (millis() - _lastChange > _delay) {
    // store the new button state
    _state = currentState;
  }
  
  // return state
  return _state;
}

