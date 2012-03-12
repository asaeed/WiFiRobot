
#include "WiFly.h"
#include "MotorDriver.h"

#define motorAPWM 11
#define motorAIN1 12
#define motorAIN2 13
#define motorBPWM 10
#define motorBIN1 9
#define motorBIN2 8

MotorDriver leftMotor = MotorDriver(motorAIN1, motorAIN2, motorAPWM);
MotorDriver rightMotor = MotorDriver(motorBIN1, motorBIN2, motorBPWM);

WiFlyClient client("192.168.1.101", 800);

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);
  
  WiFly.setUart(&Serial1);
  WiFly.begin();
  
  Serial1.println("set comm idle 2");  // close an inactive conn in 2 seconds
  WiFly.join("SecondFloor", "firstfloor99");
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
  
  getData();
}

void getData()
{
   if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected()) {
    //delay(100);
    sendRequest();
  }
}

void sendRequest()
{
  Serial.println();
  Serial.println("connecting...");
  if (client.connect()) {
    Serial.println("connected");
    delay(100); // seems to prevent an infrequent failed connection
    client.println("GET /robotServer/server.php");
    client.println();
  }   
}

