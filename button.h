#ifndef button_h
#define button_h

class Button {
  public:
    // constructor
    Button(int pin);
    // get debounced button state
    bool getState();
  private:
    // pin
    int _pin;
    // current state
    bool _state;
    // delay
    unsigned long _delay;
    // millis at last change
    unsigned long _lastChange;
};

#endif
