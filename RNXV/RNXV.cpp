
#include "Arduino.h"
#include "RNXV.h"

RNXV::RNXV()
{

}

void RNXV::setUart(Stream* uart)
{
  _uart = uart;
  
  enterCommandMode();
  
  println("set comm idle 0");
  awaitResponse("AOK");
  println("set ip proto 2");
  awaitResponse("AOK");
  
}

void RNXV::print(char* str)
{
  _uart->print(str);
  //_uart->flush();
  delay(250);
}

void RNXV::println(char* str)
{
  _uart->print(str);
  _uart->print("\r");
  delay(250);
}

void RNXV::enterCommandMode()
{
  print("$$$");
  awaitResponse("CMD");
}

void RNXV::exitCommandMode()
{
  print("exit\r");
  awaitResponse("EXIT");
}

void RNXV::sendCommand(char* cmd, char* response)
{
  enterCommandMode();
  print(cmd);
  print("\r");
  awaitResponse(response);
  exitCommandMode();
}

bool RNXV::connect(char* ssid, char* phrase)
{
  enterCommandMode();
  print("set wlan phrase "); println(phrase);
  awaitResponse("AOK");
  
  print("set wlan ssid "); println(ssid);
  awaitResponse("AOK");
  
  println("save");
  awaitResponse("AOK");
  
  println("reboot");
  awaitResponse("READY");
  
  println("join");
  awaitResponse("Associated!");

  exitCommandMode();
  
  return true;
}

bool RNXV::awaitResponse(const char* toMatch){
  awaitResponse(toMatch, 1000);
}

bool RNXV::awaitResponse(const char* toMatch, unsigned int timeOut = 1000) {
  int byteRead;
  unsigned long timeOutTarget; // in milliseconds

  for (unsigned int offset = 0; offset < strlen(toMatch); offset++) {
    timeOutTarget = millis() + timeOut; // Doesn't handle timer wrapping

    while (!_uart->available()) {
      // Wait, with optional time out.
      if (timeOut > 0) {
        if (millis() > timeOutTarget) {
          return false;
        }
      }
      delay(1); // This seems to improve reliability slightly
    }

    byteRead = _uart->read();
    delay(10);
    if (byteRead != toMatch[offset]) {
      offset = 0;
      if (byteRead != toMatch[offset]) {
        offset = -1;
      }
      continue;
    }
  }

  return true;
}
