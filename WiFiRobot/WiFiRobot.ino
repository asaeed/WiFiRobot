
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

int loopCount;
RNXV rnxv = RNXV();

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("setup begin");
  
  rnxv.setUart(&Serial1);
  Serial.println("uart set");
  
  rnxv.connect("QJ5D4","Y3DJ793Y8RZBFHYC");
  
  rnxv.sendCommand("set ip proto 1", "AOK");
  rnxv.sendCommand("set ip host 192.168.1.7", "AOK");
  rnxv.sendCommand("set ip remote 3000", "AOK");
  rnxv.sendCommand("set ip local 4000", "AOK");
  rnxv.sendCommand("save", "AOK");
  rnxv.sendCommand("reboot", "READY");
  
  rnxv.sendCommand("join", "AOK");
  
  loopCount = 0;
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
  
  sendData();
  receiveData();
}

void sendData()
{
  loopCount++;
  Serial1.print(loopCount);
  Serial1.print("here is a string of data being sent back and forth");
  delay(200);
}

void receiveData()
{
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }
}
