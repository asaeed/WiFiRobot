/*
  MotorControl.h Library for controlling a motor using PWM commands
 Created by OpenSource, July 30, 2010.
 Version 0.3

 For a description of the library, check the MotorControl.ccp file.
 */

#ifndef MotorControl_h
#define MotorControl_h

#include "WProgram.h"

class MotorControl
{
public:
  MotorControl(int pinFwd,int pinRev,int pinPwm);

  void SpeedWrite(int Speed);
  void SpeedStepUp(int PwmStepUp);
  void SpeedStepDown(int PwmStepDown);

  char Direction();
  char _Direction;

  int SpeedRead();

private:
  int _pinFwd;
  int _pinRev;
  int _pinPwm;
  int _Pwm;
  int _PwmStepUp;
  int _PwmStepDown;
  int _ConstrainPwm;
};
#endif 
