

#ifndef __CMightyLight_H__
#define __CMightyLight_H__

//#include "Arduino.h"
#include "TimeoutEvent.h"

// Light can strobe at what ever frequency is set.
class CMightyLight
{

public:
	CMightyLight();
	~CMightyLight();

  void SetupOutputPin(int nPin);

  // 2 means 0.5s on, then 0.5s off.
  // 1 gives a relaxing blink
  // 2 is attention attracting
  // 5 is alarming
  // > 10 is annoying.
  // 0 means off
  // < 0 means on
  void SetFrequency(double dFreq);

  void Pump();

private:

  void OnPulseEvent();

  int m_nOutputPin;

  // Sets frequency of blink.
  // 0 means switch off
  // 1 means 0.5s on, 0.5s off
  // 5 means on-off every 100ms.
  double m_nFrequency;

  CTimeoutEvent m_evtPulse;

  bool m_bSwitchedOn;

};

#endif //__CMightyLight_H__






