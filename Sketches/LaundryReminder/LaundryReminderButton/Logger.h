#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

class Logger
{
  public:
    Logger();
    void logError(char * errorMsg);
    void logInfo(StringSumHelper infoMsg);
    void logInfo(char * infoMsg);
    void logInfoAppend(StringSumHelper infoMsg);
    void logInfoAppend(char * infoMsg);
  
  private:
    void WriteErrorToScreen (char * msg);
    void WriteInfoToScreen (char * msg, bool append);
    const char * ERROR_TXT = "Error: " ;
    const char * INFO_TXT = "Info: " ;
};

#endif