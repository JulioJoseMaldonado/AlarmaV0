#include <Arduino.h>

//Set pìn A1, A2 how output.
void SetupRelays()
{
  DDRC |= ((1<<PC1) | (1<<PC2));
}
//Short the current of the car.
void ShortCurrent()
{
  PORTC &= ~(1<<PC1);

}
//It closes the current circuit of the car.
void ActiveCurrent()
{
  PORTC |= (1<<PC1);
}
//Active the relay that controls the horn.
void ActiveHorns()
{
  PORTC &= ~(1<<PC2);
}
//Deactivates the relay that controls the horn.
void DeactiveHorns()
{
  PORTC |= (1<<PC2);
}
