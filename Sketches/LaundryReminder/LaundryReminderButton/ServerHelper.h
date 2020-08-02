#ifndef ServerHelper_h
#define ServerHelper_h

#include <Arduino.h>

class ServerHelper
{
  public:
    ServerHelper(int pin);
    void test();
  private:
    int _pin;
};

#endif