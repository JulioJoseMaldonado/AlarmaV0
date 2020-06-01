

#include "Timer1.h"
#include "arduino.h"

/*
  Forma de calcular tiempo
      1- pasar x seg a su valor en Hz (y)
      2- Dado que estoy utilizando el prescaler 1024
            z = 16000000 / (1024*y)
      3- Calcular numero de interaciones completas, osea cuando el contador llegar a 65536
      4- Calcular el resto que me queda que no es una iteración completa.

  ej: 5 min es 300 seg  es aprox 0.0033Hz
      z = 16000000 / (1024*0.0033) = 4734848 (aproximadamente)
      numero de interaciones = 4734848 / 65536 = 72,25
      Entonces son 72 iteraciones completas.
      TCNT1 = 0x0000

*/


int numberofiteration;

void (*callback)();
void (*NeoPixelCallBack)();

//When the registry counter reach 65536 generate this ISR.
ISR (TIMER1_OVF_vect)
{
  if (numberofiteration != 0)
  {
    NeoPixelCallBack();
    TCNT1 = 0x0000;
    numberofiteration--;
  }
  else
  {
		TIMSK1 &= ~(1 << TOIE1);
		callback();
  }

}

//Start timer 1.
void Timer1_Start()
{
	TIMSK1 |= (1 << TOIE1); //Enable the interrupt the TIMER1.
}

//Stop the timer and reset internal variables.
void Timer1_StopReset()
{
  TCNT1 = 0x0000;  //Set registry counter to zero.
  numberofiteration = 7; //reset the number of iteration.
  TIMSK1 &= ~(1 << TOIE1); ////Disable the interrupt the TIMER1
}

//Set up the registers necessary for Timer 1.
//Callback: function that call when time is over.
void Timer1_init(void (*Callback)())
{
	callback = Callback;
	numberofiteration = 7; //For approximately 30 seconds
	//SetUp Timer.
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TIMSK1 = 0X00;
	TCCR1B |= ((1<<CS10) | (1 << CS12));   // Setup prescaler in 1024.
	TCNT1 = 0x0000; //Set registry counter to zero.
}

//Set callback for neopixel.
void SetupNeoPixelCallback(void (*callback)())
{
  NeoPixelCallBack = callback;
}
