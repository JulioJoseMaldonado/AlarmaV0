#include "Timer2.h"
#include "arduino.h"


void (*callback2)();
bool lastiteration2;
int numberiteration2;



//When the registry counter reach 256 generate this ISR.
ISR (TIMER2_OVF_vect)
{
	TCNT2 = 0x0000; //Set the registry counter for timer 2 to zero.
	if (numberiteration2 ==0)
	{
		callback2();
		numberiteration2 = 300;
	}
	else
	{
		numberiteration2--;
	}

}
//Start timer 2.
void Timer2_Start()
{
		TIMSK2 |= (1 << TOIE2); //Enable interrupt for Timer 2.
}

//Stop the timer 2 and reset internal variables.
void Timer2_StopReset()
{

	numberiteration2 = 300;
	TCNT2 = 0x0000; //Set the registry counter for timer 2 to zero.
	TIMSK2 &= ~(1 << TOIE2); //Disable interrupt for Timer 2.
}

//Set up the registers necessary for Timer 2.
//Callback: function that call when time is over.
void Timer2_init(void (*Callback)())
{
	numberiteration2 = 300;
	callback2 = Callback;
	//SetUp Timer 2.
	TCCR2A = 0x00;
	TCCR2B = 0x00;
	TIMSK2 = 0X00;
	TCCR2B |= ((1<<CS20) | (1 << CS22)); //Setup prescaler in 1024.
	TCNT2 = 0x0000; //Set the registry counter for timer 2 to zero.




}
