#include <Arduino.h>

#include "MoreOrLessProtocol.h"

#include "SimpleServo.h"
#include "TimeoutEvent.h"
#include "MightyLight.h"

#define SERIAL_BAUD 115200

int ledPin = 13;
 

#define LED_RED 2
#define LED_YELLOW 5
#define LED_GREEN 3
// #define LED_BLUE 5

#define SERVO_PIN 11 
#define SERVO_POWER_PIN 12 
#define TRIM_MIN A0
#define TRIM_MAX A1 

#define SLAVE_ADDR_SCORE_MARKER  0x2b
CMoreOrLessProtocol m_protocol;

CSimpleServo m_servo;

CTimeoutEvent m_evtHeartbeat;

// These are the min and max degree positions for 0 and 10 points.
// They will be offset, by up to 45° by the trim pots.
int m_servoPosMin = 45;
int m_servoPosMax = 135;

CMightyLight m_mlA;
CMightyLight m_mlB;


void PulsePin(int nPin, int msDelay)
{
  digitalWrite(nPin, HIGH);   
  delay(msDelay);             
  digitalWrite(nPin, LOW);   
}

// returns -45 to 45, which relates to trimpot position
int getTrimpotOffset(int pin)
{
  // return 0;
  analogReference(DEFAULT);
  for(int n = 0; n < 5; n++)
  {
    analogRead(pin);    
  }
  int val = analogRead(pin); 
  int nRet = map(val, 0, 1024, -45, 45);
  return nRet;
}

/*
int getTrimpotOffsetTest(int pin)
{
  // return 0;
  // analogReference(DEFAULT);
  for(int n = 0; n < 5; n++)
  {
    analogRead(pin);    
  }
  int val = analogRead(pin); 
  int nRet = map(val, 0, 1024, 1, 10);
  if(nRet < 1)
    nRet = 1;
  return nRet;
}*/

/*
void TestPots()
{
  Serial.println("TestPots.");
  int nMax = getTrimpotOffsetTest(TRIM_MAX);
  m_mlB.SetFrequency(nMax); 

  int nMin = getTrimpotOffsetTest(TRIM_MIN);
  m_mlA.SetFrequency(nMin);  

  Serial.print("max:");Serial.print(nMax);Serial.print("min:");Serial.println(nMin);
}
*/


int GetServoDegsFromScore(int nScore)
{
  int nOffsetMin = getTrimpotOffset(TRIM_MIN);
  int nOffsetMax = getTrimpotOffset(TRIM_MAX);

 // int nBlinkMin = map(nScore, -45, 45, 1, 10);
//  int nBlinkMax = map(nScore, -45, 45, 1, 10);

//  m_mlA.SetFrequency(nBlinkMin);
 // m_mlB.SetFrequency(nBlinkMax);

  int nPos = map(nScore, 10, 0, m_servoPosMin + nOffsetMin, m_servoPosMax + nOffsetMax);
  return nPos;
}

void setup() 
{ 

  Serial.begin(SERIAL_BAUD);
  delay(1000);
  while(!Serial) {} // Wait
  Serial.println("Program begins.");

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  //// pinMode(LED_BLUE, OUTPUT);


  PulsePin(LED_RED, 500);
  PulsePin(LED_YELLOW, 500);
  PulsePin(LED_GREEN, 500);
  //// PulsePin(LED_BLUE, 100);

     
  m_protocol.Setup(SLAVE_ADDR_SCORE_MARKER, true);
  m_servo.SetupOutputPin(SERVO_PIN);
  m_servo.SetServoPowerPin(SERVO_POWER_PIN);
  m_servo.SetPosition(90);

  m_evtHeartbeat.SetTimeout(10000);
  // m_ml.SetupOutputPin(LED_PIN);
  m_mlA.SetupOutputPin(LED_GREEN);
  m_mlB.SetupOutputPin(LED_YELLOW);

  // m_ml.SetFrequency(1);
  m_mlA.SetFrequency(1);
  m_mlB.SetFrequency(1);
} 
  
void loop() 
{  
 if(m_evtHeartbeat.HasTimedout()) 
  {
    m_evtHeartbeat.ResetTimer();
    PulsePin(LED_RED, 2);

    // TestPots(); 
  }  

  if(m_protocol.HasVizDataArrived())
  {
    PulsePin(LED_RED, 2);
    SVizData vd = m_protocol.GetVizDataConsuming();

    int nValueToShow = 0;
    if(vd.GameState == GamePlaying)
      nValueToShow = vd.PotentialScore;

    int nPosDegs = GetServoDegsFromScore(nValueToShow);
    Serial.print("value to show: ");Serial.print(nValueToShow);
    Serial.print(", Setting servo pos (degs):"); Serial.println(nPosDegs);

    m_servo.SetPosition(nPosDegs);
  }

  m_servo.Pump();

  m_mlB.Pump();
  m_mlA.Pump();

} 