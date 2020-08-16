#ifndef EEPROMHelper_h
#define EEPROMHelper_h

#include <Arduino.h>
#include <Preferences.h>

class EEPROMHelper
{
  public:
    EEPROMHelper(const char* ssidLocation, const char* pwLocation);
    void StorePW(String pw);
    void StoreSSID(String ssid);
    String GetPW();
    String GetSSID();
  private:
    const char* _ssidLocation;
    const char * _pwLocation;
    Preferences _preferences;
};

#endif