

#ifndef MotorDriver_h
#define MotorDriver_h

#include "WProgram.h"

class MotorDriver
{
public:
  MotorDriver(int pinFwd,int pinRev,int pinPwm);

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
