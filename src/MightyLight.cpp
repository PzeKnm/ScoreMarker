
#include "MightyLight.h"

#include <Arduino.h>




CMightyLight::CMightyLight()
{
  m_bSwitchedOn = 0;
} 


CMightyLight::~CMightyLight()
{
} 


void CMightyLight::SetupOutputPin(int nPin)
{
  m_nOutputPin = nPin;
  pinMode(m_nOutputPin, OUTPUT); 
}


void CMightyLight::SetFrequency(double dFreq)
{
  if(dFreq == m_nFrequency)
    return;
  m_nFrequency = dFreq;
  if(m_nFrequency == 0)
  {
    digitalWrite(m_nOutputPin,LOW); 
    m_bSwitchedOn = false;
    return;
  }

  if(m_nFrequency < 0)
  {
    digitalWrite(m_nOutputPin,HIGH); 
    m_bSwitchedOn = true;
    return;
  } 

  // 1000 ms / how often you want
  int timeout = (1000 / m_nFrequency);

  if(timeout < 10)
    timeout = 10;

  m_evtPulse.SetTimeout(timeout);
}


void CMightyLight::Pump()
{
  if(m_nFrequency <= 0)
    return;

  if(m_evtPulse.HasTimedout())
  {
    m_evtPulse.ResetTimer();
    OnPulseEvent();
  }
}


void CMightyLight::OnPulseEvent()
{
  if(m_bSwitchedOn)
  {
    digitalWrite(m_nOutputPin,LOW); 
    m_bSwitchedOn = false;
  }
  else
  {
    digitalWrite(m_nOutputPin,HIGH); 
    m_bSwitchedOn = true;
  } 
}









