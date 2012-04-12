
#include "Arduino.h"
#include "RNXV.h"

RNXV::RNXV()
{

}

void RNXV::setUart(Stream* uart)
{
  _uart = uart;
  enterCommandMode();
  
  //println("set comm idle 0");
  //awaitResponse("AOK");
  //println("set ip proto 2");
  //awaitResponse("AOK");
  
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

void RNXV::sendData(String str)
{
  _uart->print(str);
  delay(150);
}

char* RNXV::receiveData()
{ 
  char dataIn[100];
  dataIn[0] = '\0';  // init by setting to '\0' or 0
  int i = 0;
  while (_uart->available() > 0) {
    _uart->flush();
    char byte = _uart->read();
	if (byte == '\n')  // if end of line
	  dataIn[i] = '\0';  
	else 
	  dataIn[i] = byte;
	i++;
  }
  return dataIn;
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

bool RNXV::join(char* ssid, char* phrase)
{
  // make sure we're not in cmd mode
  exitCommandMode();
  enterCommandMode();

  println("ver");
  awaitResponse("AOK");
  
  print("set wlan ssid "); println(ssid);
  awaitResponse("AOK");

  print("set wlan phrase "); println(phrase);
  awaitResponse("AOK");

  println("save");
  awaitResponse("AOK");

  //println("reboot");
  //awaitResponse("READY");
  //delay(2000);
  
  println("join");
  awaitResponse("Associated!");

  exitCommandMode();
  
  return true;
}

void RNXV::enableUdp(char* hostIp, int hostPort, int localPort)
{
  exitCommandMode();
  enterCommandMode();
  
  println("set ip proto 1");
  awaitResponse("AOK");
  
  char buffer [33];
  
  print("set ip host "); println(hostIp);
  awaitResponse("AOK");
  
  itoa(hostPort, buffer, 10);
  print("set ip remote "); println(buffer);
  awaitResponse("AOK");
  
  itoa(localPort, buffer, 10);
  print("set ip local "); println(buffer);
  awaitResponse("AOK");
  
  println("save");
  awaitResponse("AOK");

  println("reboot");
  awaitResponse("READY");
  
  exitCommandMode();
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

const char * RNXV::ip() {
  static char ip[16] = "";
  exitCommandMode();
  enterCommandMode();
  
  println("ver");
  awaitResponse("AOK", 1000);

  println("get ip");
  awaitResponse("IP=", 1000);

  char newChar;
  byte offset = 0;
  while (offset < sizeof(ip)) {
    newChar = _uart->read();
    if (newChar == ':') {
      ip[offset] = '\x00';
      break;
    } else if (newChar != -1) {
      ip[offset] = newChar;
      offset++;
    }
  }

  // This handles the case when we reach the end of the buffer
  // in the loop. (Which should never happen anyway.)
  // And hopefully this prevents us from failing completely if
  // there's a mistake above.
  ip[sizeof(ip)-1] = '\x00';

  // This should skip the remainder of the output.
  awaitResponse(">", 1000);


  exitCommandMode();
  return ip;
}
