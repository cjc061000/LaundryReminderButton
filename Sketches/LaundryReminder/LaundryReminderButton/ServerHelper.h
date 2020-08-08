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
    void getRequestParameters(String queryParamStr, String &ssid, String &pw);
    void Urldecode2(String &dst, String src);
    int _pin;
};

#endif