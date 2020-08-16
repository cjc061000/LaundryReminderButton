#include <EEPROM.h>
#include "EEPROMHelper.h"

// include library to read and write from flash memory

EEPROMHelper::EEPROMHelper(const char* ssidLocation, const char * pwLocation)
{
  _ssidLocation = ssidLocation;
  _pwLocation = pwLocation;
  Preferences _preferences;
}

void EEPROMHelper::StorePW(String pw){
  _preferences.begin("wifi",false);
  _preferences.putString(_pwLocation,pw);
  _preferences.end();
};
void EEPROMHelper::StoreSSID(String ssid){
  _preferences.begin("wifi",false);
  _preferences.putString(_ssidLocation,ssid);
  _preferences.end();
};

String EEPROMHelper::GetPW(){
  _preferences.begin("wifi",false);
  return _preferences.getString(_pwLocation);
  //return EEPROM.readString(_pwLocation);
};

String EEPROMHelper::GetSSID(){
  _preferences.begin("wifi",false);
  return _preferences.getString(_ssidLocation);
  //return EEPROM.readString(_ssidLocation);
};