
#include "RNXV.h"
#include "MotorDriver.h"

#define motorAPWM 11
#define motorAIN1 12
#define motorAIN2 13
#define motorBPWM 10
#define motorBIN1 9
#define motorBIN2 8

MotorDriver leftMotor = MotorDriver(motorAIN1, motorAIN2, motorAPWM);
MotorDriver rightMotor = MotorDriver(motorBIN1, motorBIN2, motorBPWM);

int inByte= -1;                   // incoming byte from serial RX
const int inStringLength = 50;    // max length is 19 and then 1 for the termination 0?
char inString[inStringLength];    // string for incoming serial data
int stringPos = 0;                // string index counter

RNXV rnxv = RNXV();

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("setup begin");
  
  rnxv.setUart(&Serial1);
  Serial.println("uart set");
  
  
  if (rnxv.join("SecondFloor", "firstfloor99"))
  {
    Serial.println("connected");
  } else {
    Serial.println("disconnected");
  }
  
  //rnxv.sendCommand("set comm idle 0"); // if this is 0, web browser connection remains open, 2 is enough time to return response
  //rnxv.sendCommand("set ip proto 2"); // gets rid of some extra head data returned
  
  //rnxv.sendCommand("ver");
  //rnxv.sendCommand("reboot", "*READY*"); 
  //rnxv.sendCommand("get com");
  
  rnxv.enterCommandMode();
  
  Serial.println("setup end");
}

void loop()
{
  /*
  for (int i=-20; i<20; i++) {
    leftMotor.setSpeed(i);
    rightMotor.setSpeed(i);
    delay(20);
  }
  
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
  delay(1000);
  */
  
  //handleSerial();
  
  
  while(Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }
  
  while(Serial.available()) { // Outgoing data
    Serial1.write(Serial.read());
  }
  
}


void handleSerial(){
  if(Serial.available()) { // Outgoing data
    Serial1.write(Serial.read());
  }
  
  if (Serial1.available() > 0) {
    inByte = Serial1.read();
    Serial.write(inByte);
    
    if (inByte != '\n') {
      inString[stringPos] = inByte;
      stringPos++;
    }
    
    if (inByte == '\n') {
      inString[stringPos] = 0;
      //Serial.print("inString: "); Serial.println(inString);
    
      if (stringPos <= 1) {
      //if (inString == "*CLOS*"){  // waiting for this is too late to return a response
        Serial1.println("HTTP/1.1 200 OK");
        Serial1.println("Content-Type: text/html");
        Serial1.println();
        Serial1.println("Request Done."); 
        
        // this seems to reset the rn-xv so no good
        //rnxv.enterCommandMode();
        //rnxv.print("close\r");
        //rnxv.exitCommandMode();
      }
    
      // reset
      for (int c = 0; c < inStringLength; c++) {
        inString[c] = ' ';
      } 
      stringPos = 0;
    }
  } else {
    
  }
}
