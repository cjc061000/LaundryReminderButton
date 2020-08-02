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
    String getRequestUrl(String header, String &detectedParams);
    void getRequestParameters(String header, String ssid, String pw);
    int _pin;
};

#endif