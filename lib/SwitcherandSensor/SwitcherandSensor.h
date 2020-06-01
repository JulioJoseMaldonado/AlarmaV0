
//Set the interrupt registry that corresponds to the doors switchers  and indicates the procedure to call when the interrupt is generated.
//The pins for the switchers they are D4, D5.
void SetupDoorSwitcher(void (*callback)());
//Set the interrupt registry that corresponds to the boonet switcher and indicates the procedure to call when the interrupt is generated.
//The pin for the boonet switch is D2.
void SetupBoonetSwitcher(void (*callback)());
//Set the interrupt registry that corresponds to the spare tire magnetic switchand indicates the procedure to call when the interrupt is generated.
//The pin for the spare tires magnetic switch is D3.
void SetupSpareTireSwitcher(void (*callback)());
//Set the interrupt registry that corresponds pir sensor and indicates the procedure to call when the interrupt is generated.
//The pin for the Pir sensor is D8.
void SetupPirSensor(void (*callback)());
//Set the interrupt registry that corresponds for the vibration sensor and indicates the procedure to call when the interrupt is generated.
//The pin for the vibration sensor is A0.
void SetupVibrationSensor(void (*callback)());
//Set the registry for the button that permit the lecture RC522.
//The pin for read the status of the button is D7.
void SetupButtonLecture();
//Indicates the button lecture is being pressed. True when that occurs.
bool ButtonLecture();
//Active all interrupt that corresponds a switchers and sensors.
void ActiveInterrup();
//Deactive all interrupt that corresponds a switchers and sensors.
void StopInterrup();
