//Initalize i2c protocol, MPU6050 and internal variable in init values.
void SetupMPU6050();
//Indicates if there is a problem with this sensor.
//Return the state of sensor. False indicate that all is alright, True the other case.
bool ReadMPU6050();
//Put the internal variables in the initial values.
void ResetMPU6050();
