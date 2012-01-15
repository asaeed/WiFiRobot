/*
**General:**
 This library is created to control a motor (or motors) using the "Pololu TB6612FNG Dual Motor Driver Carrier"
 This library can off course also be used for any other driver that uses the same principle.
 The library only controls one motor, but it is possible to create multiple instances of the MotorControl.

 Created by OpenSource, July 30, 2010.
 Version 0.3

 **Working principle of the motor driver:**
 The driver uses two digital pins to set the direction of one motor.
 DigitalWrite(_pinFwd, HIGH) and digitalWrite(_pinRev, LOW) will set the motor in forward direction.
 DigitalWrite(_pinFwd, LOW) and digitalWrite(_pinRev, HIGH) will set the motor in reverse direction.
 The driver uses one PWM pin to set the speed of the set direction for one motor.

 To summarize: For one motor one instance of the library, two digital pins and one PWM pin is necessary.
 For the control of two motors, two instances of the library, 4 digital pins and 2 PWM pins are necessary.

 **Create a MotorControl instance:**
 MotorControl MotorLeft(5,6,7);

 In this case pin 5 and 6 are the pins to set the direction of the motor (I used the Arduino Mega):
 -  If 5 = HIGH and 6 = LOW then the direction will be forward.
 -  If 6 = LOW and 6 = HIGH then the direction will be reverse.
 -  Pin 7 in this example is the PWM pin.

 **MotorControl members:**
 The MotorControl library consists of the following members::

 1. SpeedWrite(int)
 This parameter let you directly write a speed value (255 to -255) to the motor.
 Speed = 255 means full speed forward, Speed = -255 means full speed backwards and speed = 0 means no speed.

 The speed values are actually derived from PWM values but it is not allowed to set negative PWM value's.
 The speed values are converted into PWM values.
 This value is used to write the PWM value to the driver (see the SpeedWrite code on how this principle works).

 The SpeedWrite(int) member is used in the SpeedStepUp(int) and the SpeedStepDown(int) members.

 2. SpeedStepUp(int)
 -  This parameter let you step up the motor speed in forward position.
 Example: If the speed value is 20 and SpeedStepUp(20) is set the new speed value will be: 20 + 20 = 40
 -  If the motor is in reverse it will slow down.
 Example: If the speed value is -40 and SpeedStepUp(20) is set the new speed value will be: -40 + 20 = -20

 3. SpeedStepDown(int)
 -  This parameter let you speed up the motor in reverse position.
 -  If the motor is going forward it will slow down.

 4. SpeedRead()
 This parameter will let you read the current speed of the motor.

 5. MotorDirection()
 This parameter will let you read the current direction of the motor
 - F = Forward
 - R = Reverse
 - I = Idle (PWM speed = 0)
 */

#include "WProgram.h"
#include "MotorControl.h"

MotorControl::MotorControl(int pinFwd,int pinRev, int pinPwm)
{
  pinMode(pinFwd, OUTPUT);
  pinMode(pinRev, OUTPUT);
  _pinFwd = pinFwd;
  _pinRev = pinRev;
  _pinPwm = pinPwm;
}

void MotorControl::SpeedWrite(int Pwm)
{
  _Pwm = Pwm;

  // Check if PWM value do not exceed the boundaries (-255 to 255).
  _Pwm = constrain(_Pwm, -255, 255);

  // Is the PWM value positive, if yes go forward.
  if (_Pwm >= 1 && _Pwm <= 255)
  {
    digitalWrite(_pinFwd, HIGH );
    digitalWrite(_pinRev, LOW);
    analogWrite (_pinFwd, _Pwm);
    _Direction = 'F';
  }

  // Is the PWM value zero, if yes the motor is idle.
  else if (_Pwm == 0)
  {
    digitalWrite(_pinFwd, HIGH);
    digitalWrite(_pinRev, LOW);
    analogWrite (_pinFwd, _Pwm);
    _Direction = 'I';
  }

  // Is the PWM value negative, if yes go reverse.
  else if (_Pwm >= -255 && _Pwm <= -1)
  {
    digitalWrite(_pinFwd, LOW);
    digitalWrite(_pinRev, HIGH);
    //Here the negative speed value is converted into a PWM value.
    analogWrite (_pinPwm, -_Pwm);
    _Direction = 'R';
  }
}

void MotorControl::SpeedStepUp(int PwmStepUp)
{
  _PwmStepUp = PwmStepUp;

  _Pwm = _Pwm + _PwmStepUp;

  this->SpeedWrite(_Pwm);
}

void MotorControl::SpeedStepDown(int PwmStepDown)
{
  _PwmStepDown = PwmStepDown;

  _Pwm = _Pwm - _PwmStepDown;

  this->SpeedWrite(_Pwm);
}

int MotorControl::SpeedRead()
{
  return _Pwm;
}

char MotorControl::Direction()
{
  return _Direction;
} 
