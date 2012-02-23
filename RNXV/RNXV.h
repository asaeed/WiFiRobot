
#ifndef RNXV_h
#define RNXV_h

#include "Arduino.h"

class RNXV
{
public:
  RNXV();
  void setUart(Stream* uart);
  void print(char* str);
  void println(char* str);
  void enterCommandMode();
  void exitCommandMode();
  void sendCommand(char* cmd, char* response);
  bool connect(char* ssid, char* phrase);
  //char* clientGet(char* server, unsigned int port, char* request);
  bool awaitResponse(const char* toMatch, unsigned int timeOut);
  bool awaitResponse(const char* toMatch);

private:
  Stream* _uart;

};
#endif 
