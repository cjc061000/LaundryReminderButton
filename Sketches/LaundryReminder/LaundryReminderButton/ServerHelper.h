#ifndef ServerHelper_h
#define ServerHelper_h

#include <Arduino.h>
#include <WiFi.h>

class ServerHelper
{
  public:
    ServerHelper();
    void handleNewClient(WiFiClient client, String Header, String &requestUrl, String &queryParamsStr);
    void GetRequestParameters(String queryParamStr, String &ssid, String &pw);
    void PrintHomePage(WiFiClient client);
    void PrintSubmitPage(WiFiClient client, String ssid, String pw);
  private:
    String getRequestUrl(String header, String &detectedParams);
    void Urldecode2(String &dst, String src);
    int _pin;
};

#endif