#include "SwitcherandSensor.h"
#include <Arduino.h>

void (*doorCallback)();
void (*boonetCallback)();
void (*spareTireCallback)();
void (*pirSensorCallback)();
void (*vibrationSensorCallback)();

//interrupt service routine for the doors switchers.
ISR(PCINT2_vect)
{
  PCICR &= ~(1<<PCIE2); //Disable the interrupt.
  doorCallback();
}

ISR(PCINT1_vect)
{

  vibrationSensorCallback();
}

ISR(PCINT0_vect)
{
  PCICR &= ~(1<<PCIE0); //Disable the interrupt.
  pirSensorCallback();
}


ISR (INT0_vect)
{
  EIMSK &= ~(1<< INT0); //Disable the interrupt.
  boonetCallback();
}


ISR(INT1_vect)
{
  EIMSK &= ~(1<< INT1); //Disable the interrupt.
  spareTireCallback();
}


//Set the registry for the button that permit the lecture RC522.
//The pin for read the status of the button is D7.
void SetupButtonLecture()
{
  DDRD &= ~(1<< DDD7);
}

//Set the interrupt registry that corresponds to the doors switchers  and indicates the procedure to call when the interrupt is generated.
//The pins for the switchers they are D4, D5.
void SetupDoorSwitcher(void (*callback)())
{
  DDRD &= ~(1<< DDD4);
  DDRD &= ~(1<< DDD5);
  PCMSK2 |= ((1<<PCINT20)| (1<<PCINT21));
  doorCallback = callback;
}

//Set the interrupt registry that corresponds to the boonet switcher and indicates the procedure to call when the interrupt is generated.
//The pin for the boonet is D2.
void SetupBoonetSwitcher(void (*callback)())
{
  DDRD &= ~(1<< DDD2);
  EICRA |= (1<< ISC00);
  boonetCallback = callback;
}

//Set the interrupt registry that corresponds to the spare tire magnetic switchand indicates the procedure to call when the interrupt is generated.
//The pin for the spare tires magnetic switch is D3.
void SetupSpareTireSwitcher(void (*callback)())
{
  DDRD &= ~(1<< DDD3);
  EICRA |= (1<< ISC10);
  spareTireCallback = callback;
}

//Set the interrupt registry that corresponds pir sensor and indicates the procedure to call when the interrupt is generated.
//The pin for the Pir sensor is D8.
void SetupPirSensor(void (*callback)())
{
  DDRB &= ~(1<<DDB0);
  PCMSK0 |= (1<<PCINT0);
  pirSensorCallback = callback;
}

//Set the interrupt registry that corresponds for the vibration sensor and indicates the procedure to call when the interrupt is generated.
//The pin for the vibration sensor is A0.
void SetupVibrationSensor(void (*callback)())
{
  DDRC &= ~(1<<DDC0);
  PCMSK1 |= (1<<PCINT8);
  vibrationSensorCallback = callback;
}

//Active all interrupt that corresponds a switchers and sensors.
void ActiveInterrup()
{
  PCIFR |= ((1<<PCIF2) | (1<<PCIF1) | (1<<PCIF0)); //Clear the PCINT interrupt flags.
  PCICR |= ((1<<PCIE2) | (1<<PCIE1) | (1<<PCIE0)); //Actived the PCINT interrupt.
  EIFR |= ((1<<INTF0) | (1<<INTF1)); //Clear the INT interrupt flags.
  EIMSK |= ((1<<INT0) | (1<<INT1)); //Actived the INT interrupt.
}

//Deactive all interrupt that corresponds a switchers and sensors.
void StopInterrup()
{

  PCICR &= ~((1<<PCIE2) | (1<<PCIE1) | (1<<PCIE0)); //Disable the interrupt.
  EIMSK &= ~((1<< INT1) | (1<< INT0)); //Disable the interrupt.
}

//Indicates the button lecture is being pressed. True when that occurs.
bool ButtonLecture()
{
  if (!(PIND & (1<< PD7))) //Read registry for know the status of button.
  {return true;}
  return false;
}
