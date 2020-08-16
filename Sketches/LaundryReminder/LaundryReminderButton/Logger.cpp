#include <Arduino.h>
#include "Logger.h"
#include "common.h"

Logger::Logger(){
}
void Logger::WriteErrorToScreen (char * msg){
  display.clearDisplay();
  display.setCursor(0,0);
  display.write(ERROR_TXT);
  display.write(msg);
  display.display();
  delay(100);
}
void Logger::WriteInfoToScreen (char * msg, bool append){
  if(!append){
    display.clearDisplay();
    display.setCursor(0,0);
    display.write(INFO_TXT);
  }
  display.write(msg);
  display.display();
  delay(100);
}

void Logger::logError(char * errorMsg){ 
  // write into the serial port
  Serial.println(ERROR_TXT);
  Serial.print(errorMsg);

  // write into the screen
  WriteErrorToScreen(errorMsg);
}
void Logger::logInfo(StringSumHelper infoMsg){
    logInfo( strdup( infoMsg.c_str()) );
}
void Logger::logInfo(char * infoMsg){ 
  // write into the serial port
  Serial.println(INFO_TXT);
  Serial.print(infoMsg);

  // write into the screen
  WriteInfoToScreen(infoMsg, (bool)false);
}
void Logger::logInfoAppend(StringSumHelper infoMsg){
    logInfoAppend( strdup( infoMsg.c_str()) );
}
void Logger::logInfoAppend(char * infoMsg){
  // write into the serial port
  Serial.println(INFO_TXT);
  Serial.print(infoMsg);

  // write into the screen
  WriteInfoToScreen(infoMsg, (bool)true);
}
