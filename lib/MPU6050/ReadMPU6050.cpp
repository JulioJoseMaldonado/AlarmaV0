#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"


MPU6050 mpu(0x68); //variable that hanble the MPU6050, i2c addres = 0x68.
bool firstread, vibration; //firstread is the initial values.
                           //vibration is used for avoid vibration situations.
float init_ang_x, init_ang_y; //Inital values for the angles when the alarma is actived.
int init_ax, init_ay, init_az;



//Indicates if there is a problem with this sensor.
//Return the state of sensor. False indicate that all is alright, True the other case.
bool ReadMPU6050()
{
  bool notok = false;
  float ang_x, ang_y; //Currents angles.
  int ax, ay, az; //Current accelerations.
  mpu.getAcceleration(&ax, &ay, &az); //Read the accelerations.
  //Calculate the current angles.
  ang_x = abs(atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14));
  ang_y = abs(atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14));
  if (firstread) //If first read setup the initial angles values.
  {
    init_ang_x = ang_x;
    init_ang_y = ang_y;
    init_ax = abs(ax);
    init_ay = abs(ay);
    init_az = abs(az);
    firstread = false;
  }
  else
  {
    //Calculate the difference between current and initial angles.
    float Dx = abs(ang_x - init_ang_x);
    float Dy = abs(ang_y - init_ang_y);
    if ((Dx>1.5) || (Dy>1.5)) //Detect a problem with the differences.
    {
      Serial.println("Vibration.");
      if (vibration) {vibration = false;} //Prevents that difference doen´t because a vibration.
      else {notok = true; } //The difference is not due to vibrations.
    }
    else {vibration = true;}


  }
  return (notok); //Return the state of sensor. False indicate that all is alright, True the other case.
}

//Put the internal variables in the initial values.
void ResetMPU6050()
{
  vibration = true;
  firstread = true;
}

//Initalize i2c protocol, MPU6050 and internal variable in init values.
void SetupMPU6050()
{
   firstread = true;
   vibration = true;
   Wire.begin();
   mpu.initialize();
}
