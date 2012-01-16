
#include "MotorDriver.h"

#define out_A_PWM 11
#define out_A_IN1 12
#define out_A_IN2 13
#define out_B_PWM 10
#define out_B_IN1 9
#define out_B_IN2 8

MotorDriver leftMotor = MotorDriver(12, 13, 11);
MotorDriver rightMotor = MotorDriver(9, 8, 10);

void setup()
{
  Serial.begin(9600);
  Serial.println("begin");
  
}

void loop()
{
  for (int i=-20; i<20; i++) {
    leftMotor.setSpeed(i);
    rightMotor.setSpeed(i);
    delay(20);
  }
  
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
  delay(1000);
}

