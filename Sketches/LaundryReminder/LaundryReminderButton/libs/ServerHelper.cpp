
#include "Arduino.h"
#include "ServerHelper.h"

ServerHelper::ServerHelper(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void ServerHelper::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void ServerHelper::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}