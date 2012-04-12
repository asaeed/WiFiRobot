
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
  void sendData(String str);
  char* receiveData();
  void enterCommandMode();
  void exitCommandMode();
  void sendCommand(char* cmd, char* response);
  bool join(char* ssid, char* phrase);
  //char* clientGet(char* server, unsigned int port, char* request);
  bool awaitResponse(const char* toMatch, unsigned int timeOut);
  bool awaitResponse(const char* toMatch);
  void enableUdp(char* hostIp, int hostPort, int localPort);
  const char* ip();

private:
  Stream* _uart;

};
#endif 
