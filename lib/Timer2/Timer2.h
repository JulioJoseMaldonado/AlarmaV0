
//Set up the registers necessary for Timer 2.
//Callback: function that call when time is over.
void Timer2_init(void (*Callback)());

//Start timer.
void Timer2_Start();

//Stop the timer 2 and reset internal variables.
void Timer2_StopReset();
