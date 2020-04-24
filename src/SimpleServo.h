

#ifndef __CSimpleServo_H__
#define __CSimpleServo_H__

//#include "Arduino.h"
#include "TimeoutEvent.h"
#include <Servo.h>

class CSimpleServo
{

public:
	CSimpleServo();
	~CSimpleServo();

  void SetServoPowerPin(int nPin);

  void SetupOutputPin(int nPin);

  // Sets the servo position in degrees
  void SetPosition(int nDegs);

  void SetPulseMinMax(int min, int max);

  void Pump();

private:

  void OnPulseEvent();


  // The pin that outputs the PWM signal
  int m_nOutputPin;

  int m_nPositionDegs;

  int m_nPulseLenMicroS;

  // The pin that allows power to the servo the prevent twiching on start.
  int m_nPowerPin;

  CTimeoutEvent m_evtPulse;

  // The theroretical min max for the servo, although it often can go beyond this.
  int m_nPulseMin = 800;
  int m_nPulseMax = 2200;

  CTimeoutEvent m_evtPowerOn; 
  bool m_bPowerOn;

  bool m_bUseArduinoLib;
  Servo myServo;
};

#endif //__CSimpleServo_H__






