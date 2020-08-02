#ifndef ServerHelper_h
#define ServerHelper_h

#include <Arduino.h>
#include <WiFi.h>

class ServerHelper
{
  public:
    ServerHelper();
    void handleNewClient(WiFiClient client, String Header);
  private:
    int _pin;
};

#endif