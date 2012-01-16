
#include "WProgram.h"
#include "MotorDriver.h"

MotorDriver::MotorDriver(int pinFwd,int pinRev, int pinPwm)
{
  pinMode(pinFwd, OUTPUT);
  pinMode(pinRev, OUTPUT);
  _pinFwd = pinFwd;
  _pinRev = pinRev;
  _pinPwm = pinPwm;
}

void MotorDriver::SpeedWrite(int Pwm)
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

void MotorDriver::SpeedStepUp(int PwmStepUp)
{
  _PwmStepUp = PwmStepUp;

  _Pwm = _Pwm + _PwmStepUp;

  this->SpeedWrite(_Pwm);
}

void MotorDriver::SpeedStepDown(int PwmStepDown)
{
  _PwmStepDown = PwmStepDown;

  _Pwm = _Pwm - _PwmStepDown;

  this->SpeedWrite(_Pwm);
}

int MotorDriver::SpeedRead()
{
  return _Pwm;
}

char MotorDriver::Direction()
{
  return _Direction;
} 
