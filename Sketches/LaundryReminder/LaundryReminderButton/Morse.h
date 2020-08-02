#ifndef Morse_h
#define Morse_h

#include <Arduino.h>

class Morse
{
  public:
    Morse(int pin);
    void test();
  private:
    int _pin;
};

#endif