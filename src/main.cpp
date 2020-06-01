#include <Arduino.h>
#include <ReadRC522.h>
#include "SwitcherandSensor.h"
#include "ReadMPU6050.h"
#include "NeoPixelDisplay.h"
#include "RelaysManager.h"
#include "Timer2.h"
#include "Timer1.h"
#include "EEPROM.h"


bool alarmActive;
bool horns, turnonHorns, swichDetected, waitingTimer1Out, openDoor, turnon;
unsigned long starTime;
int counthorn, iterationhorn;
const int direcionAlarmDeactive = 0;
const int direcionAlarmActiveSound = 1;


/*------     Support functions     ------*/
//Set and call the functions necessary for sound de horn.
void TurnOnHorns()
{
  Serial.println("Horns");
  horns = true;
  TurnOnNeoPixelRed();
  ShortCurrent();
  Timer2_Start(); //Control when sound o not sound the horns.
  Timer1_StopReset();
  EEPROM.write(direcionAlarmActiveSound, 1); //Save state in a eeprom in case the system turn off.
}

//Establish internal values and values of the module to the state deactivated alarm.
void AlarmOff()
{
  Timer1_StopReset();
  Timer2_StopReset();
  TurnOnNeoPixelGreen();
  ResetMPU6050();
  StopInterrup();
  DeactiveHorns();
  ActiveCurrent();
  swichDetected = false;
  openDoor = false;
  horns=false;
  iterationhorn = 0;
  counthorn = 0;
  turnonHorns = true;
}

//Is responsible  the disable the alarm.
void deactivateAlarm()
{
  bool exit = false;
  Serial.println("Esperando Tag para desactivar.");
  while(!exit) //Repeat while don't have a tag valid.
  {
    if (ReadTag() && ButtonLecture()) //Only control if a tag valid  if have a present tag and detect the button lecture is pressed.
    {
      Serial.println("Alarma Desactivada.");
      alarmActive = false;
      exit = true;
      EEPROM.write(direcionAlarmDeactive, 1); //Save state in a eeprom in case the system turn off.
      EEPROM.write(direcionAlarmActiveSound, 0); //Save state in a eeprom in case the system turn off.

    }
  }
  AlarmOff();
}

//Is responsible  the enable the alarm.
void ActiveAlarm()
{
  Serial.println("Esperando Tag.");
  if (ReadTag()) //Control if that read tag valid.
  {
      Serial.println("Tag ingresado.");
      Timer1_Start(); //Init timer 1 for give time a the occupants the car get out.
      EEPROM.write(direcionAlarmDeactive, 0); //Save state in a eeprom in case the system turn off.
      TurnOffNeoPixel();
      TurnOnNeoPixelBlue();
      waitingTimer1Out = true;
  }
}


/*------ Callbacks for interrups -------*/

/* -- Switch and Sensor -- */

void callbackBoonet()
{
  Serial.println("Bonnet.");
  swichDetected = true;
  if(!horns)
  {
    TurnOnHorns();
  }
}

void callbackSpareTire()
{
  Serial.println("Tire.");
  swichDetected = true;
  if(!horns)
  {
    TurnOnHorns();
  }
}

void callbackPirSensor()
{
  Serial.println("Pir.");
  swichDetected = true;
  if(!horns)
  {
    TurnOnHorns();
  }
}

void callbackDoor()
{
  Serial.println("Door.");
  swichDetected = true;
  if (!horns) { Timer1_Start();}
  openDoor = true;
}

/* -- Timers -- */

void callbackTimer2Out()
{
  if (counthorn<10) //Only the sound ten time for iteration.
  {
      if (turnonHorns) { ActiveHorns();}
      else { DeactiveHorns();}
      turnonHorns = !turnonHorns;
      counthorn++;
  }
  else
  {
      if (iterationhorn <=3)
      {
          if (counthorn == 20) //Repeat ten times without sound horns.
          {
            counthorn = 0; //Reset for a new iteration.
            iterationhorn++; //Increment the number of iteration.
          }
          else
          { counthorn++;}

       }
       else {Timer2_StopReset();} //Stop timer 2 for don't sound more the horns.
   }
}

void callbackTimer1Out()
{
    Serial.println("Alarma Activa.");
    if (!alarmActive)  //Active the alarm.
    {
      Serial.println("Alarma Activa.");
      alarmActive = true;
      openDoor = false;
      waitingTimer1Out = false;
      starTime = millis();
      ActiveInterrup();
    }
    else if (openDoor && !horns) {TurnOnHorns();}
    Timer1_StopReset();
}

void callbackNeoPixelTimer1()
{
    if (turnon) {TurnOnNeoPixelBlue();}
    else {TurnOffNeoPixel();}
    turnon = !turnon;
}

/* ------ Initializer for each case ------ */

void InitBase()
{
  openDoor = false;
  turnonHorns = true;
  turnon = false;
  counthorn = 0;
  iterationhorn = 0;
  waitingTimer1Out = false;
  cli();
  Timer1_init(callbackTimer1Out);
  Timer2_init(callbackTimer2Out);
  SetupRC522();
  SetupNeoPixelCallback(callbackNeoPixelTimer1);
  SetupDoorSwitcher(callbackDoor);
  SetupBoonetSwitcher(callbackBoonet);
  SetupSpareTireSwitcher(callbackSpareTire);
  SetupPirSensor(callbackPirSensor);
  SetupButtonLecture();
  SetupRelays();
  DeactiveHorns();
  ActiveCurrent();
  sei();
  SetupNeoPixel();
  TurnOnNeoPixelGreen();
  SetupMPU6050();
}

void InitVarAlarmDesactive()
{
  alarmActive = false;
  swichDetected = false;
  InitBase();

}

void InitAlarmActive()
{
  alarmActive = true;
  swichDetected = false;
  starTime = millis();
  InitBase();
  ActiveInterrup();
}

void InitActiveSound()
{
  alarmActive = true;
  swichDetected = true;
  InitBase();
  TurnOnHorns();
}





/*-------------------------------------------*/

void setup() {
    Serial.begin(9600);
    Serial.println("setup");
    if (EEPROM.read(direcionAlarmDeactive)){Serial.println("0"); InitVarAlarmDesactive();} //Turn on in the state deactivaded.
    else if (EEPROM.read(direcionAlarmActiveSound)) {Serial.println("1"); InitActiveSound(); } //Turn on in the state activaded and sound horns.
    else {Serial.println("2"); InitAlarmActive();} //Turn on in the state activaded.

}

void loop() {

  if ((!alarmActive) && ButtonLecture() && !waitingTimer1Out) //La alarma esta desactivada.
  {
    ActiveAlarm();
  }
  else if(alarmActive) //La alarma esta activada.
  {
    if (swichDetected || ButtonLecture())
    {
      deactivateAlarm();
    }
    else if (millis()-starTime>2000) {
          if (ReadMPU6050())
          {
            Serial.println("MPU6050");
            swichDetected = true;
            if(!horns){TurnOnHorns();}
          }
          else
          {
            if (turnon) {TurnOnNeoPixelRed();}
            else {TurnOffNeoPixel();}
            turnon = !turnon;
          }
          starTime = millis();
        }
  }

}
