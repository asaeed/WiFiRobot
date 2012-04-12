/*
 *  WiFiRobot
 *  
 *
 *  3/1/2012
 *  Ahmad Saeed 
 *  www.niltoid.com
 */
 
#include "Wire.h"
#include "HMC5883L.h"
#include "RNXV.h"
#include "MotorDriver.h"
#include "aJSON.h"
#include "Servo.h"

#define motorAPWM 11
#define motorAIN1 12
#define motorAIN2 13
#define motorBPWM 10
#define motorBIN1 9
#define motorBIN2 8

#define neckMotorPin 7 // pwm only?

MotorDriver leftMotor = MotorDriver(motorAIN1, motorAIN2, motorAPWM);
MotorDriver rightMotor = MotorDriver(motorBIN1, motorBIN2, motorBPWM);
HMC5883L compass = HMC5883L();
RNXV rnxv = RNXV();
const char* ip;

Servo neckMotor;
float neckAngle;
char json[200];
String jsonString;
aJsonObject* root, * neck;

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  Serial.println("setup begin");
  
  setupHMC5883L();
  neckMotor.attach(neckMotorPin);
  neckAngle = 90;
  neckMotor.write(neckAngle-1);
  
  rnxv.setUart(&Serial1);
  Serial.println("uart set");
  
  rnxv.join("SecondFloor","firstfloor99");
  rnxv.enableUdp("192.168.1.7", 3000, 4000);
  rnxv.sendCommand("join", "AOK");
  ip = rnxv.ip();
  
  Serial.println(ip);
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
  
  float heading = getHeading();
  
  String data = "{'ip':'";
  data += ip;
  data += "', 'heading':";
  data += (int)heading;
  data += "}";
  
  rnxv.sendData(data);
  char * dataIn = rnxv.receiveData();
  if (dataIn[0] != 0){
    Serial.println(dataIn);
    jsonString = String(dataIn);
    // without this if statement, ext power causes freeze
    if (jsonString.charAt(0) == '{' && jsonString.charAt(jsonString.length()-1) == '}'){
      json[0] = 0;
      jsonString.toCharArray(json,128);
      root = aJson.parse(json);
      neck = aJson.getObjectItem(root, "neck"); 
      float neckAngle = neck->valuefloat;
      aJson.deleteItem(root);
      neckMotor.write(neckAngle);
    }
  }
}

void setupHMC5883L(){
 int error; 
 error = compass.SetScale(1.3);
 if(error != 0) Serial.println(compass.GetErrorText(error));
 error = compass.SetMeasurementMode(Measurement_Continuous);
 if(error != 0) Serial.println(compass.GetErrorText(error));
}

float getHeading(){
 MagnetometerScaled scaled = compass.ReadScaledAxis();
 float heading = atan2(scaled.YAxis, scaled.XAxis);
 if(heading < 0) heading += 2*PI;
 if(heading > 2*PI) heading -= 2*PI;
 return heading * RAD_TO_DEG;
}
