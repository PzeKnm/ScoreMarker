
#include "SimpleServo.h"

#include <Arduino.h>




CSimpleServo::CSimpleServo()
{
  m_bPowerOn = false;
  m_bUseArduinoLib = true;
} 


CSimpleServo::~CSimpleServo()
{
} 


void CSimpleServo::SetServoPowerPin(int nPin)
{
  m_nPowerPin = nPin;
  pinMode(m_nPowerPin, OUTPUT); 
  digitalWrite(m_nPowerPin,LOW);
}


void CSimpleServo::SetupOutputPin(int nPin)
{
  if(m_bUseArduinoLib)
  {
    myServo.attach(nPin, m_nPulseMin, m_nPulseMax);
    myServo.write(90);
    m_evtPowerOn.SetTimeout(5000);
  }
  else
  {
    m_nOutputPin = nPin;
    pinMode(m_nOutputPin, OUTPUT); 
    m_evtPulse.SetTimeout(19);    
  }
  
}

void CSimpleServo::SetPulseMinMax(int min, int max)
{
  m_nPulseMin = min;
  m_nPulseMax = max;
}


void CSimpleServo::SetPosition(int nDegs)
{
  if(m_bUseArduinoLib)
  {
    myServo.write(nDegs);
  }
  else
  {
    m_nPositionDegs = nDegs;
    m_nPulseLenMicroS = map(m_nPositionDegs, 0, 180, m_nPulseMin, m_nPulseMax);
  }  

}


void CSimpleServo::Pump()
{
  if(!m_bUseArduinoLib)
  {
    if(m_evtPulse.HasTimedout())
    {
      m_evtPulse.ResetTimer();
      OnPulseEvent();
    }    
  }

  if(m_bPowerOn == false && m_evtPowerOn.HasTimedout())
  {
    m_bPowerOn = true;
    Serial.println("Powering on servo.");
    digitalWrite(m_nPowerPin,HIGH);
  }  
}

void CSimpleServo::OnPulseEvent()
{
  digitalWrite(m_nOutputPin, HIGH);                // Set pin high to start pulse
  delayMicroseconds(m_nPulseLenMicroS);                // High pulse angle data   
  digitalWrite(m_nOutputPin,LOW);                  // Set low for the rest of pulse
  // delayMicroseconds(2000 - uS);  

  m_evtPowerOn.SetTimeout(5000);
}









