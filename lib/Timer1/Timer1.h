//Set up the registers necessary for Timer 1.
//Callback: function that call when time is over.
void Timer1_init(void (*Callback)());
//Start timer 1.
void Timer1_Start();
//Stop the timer and reset internal variables.
void Timer1_StopReset();

//Set callback for neopixel.
void SetupNeoPixelCallback(void (*callback)());
