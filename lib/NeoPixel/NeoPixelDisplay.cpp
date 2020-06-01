#include <Adafruit_NeoPixel.h>


//Setup values of the neopixel module.
//1, indicate that is one neopixel.
//6, pin that use for communicate with the neopixel led.
//NEO_GRB + NEO_KHZ800, type of neopixel and that frecuency that is use for communicate.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);

//Initialize neopixel.
void SetupNeoPixel()
{
  strip.begin();
  strip.show(); //Orders a neopixel that shows the changes previously sent.
}

//Turn on neopixel in red.
void TurnOnNeoPixelRed()
{
  strip.setPixelColor(0, 200,20,20,90); //set color en red.
  strip.show(); //Orders a neopixel that shows the changes previously sent.
}
//Turn on neopixel in blue.
void TurnOnNeoPixelBlue()
{
  strip.setPixelColor(0, 20,20,200,90); //set color en blue.
  strip.show();//Orders a neopixel that shows the changes previously sent.
}
//Turn on neopixel in green.
void TurnOnNeoPixelGreen()
{
  strip.setPixelColor(0, 20,200,20,90);//set color en green.
  strip.show();//Orders a neopixel that shows the changes previously sent.
}
//Turn off neopixel
void TurnOffNeoPixel()
{
  strip.clear(); //turn off the neopixel.
  strip.show(); //Orders a neopixel that shows the changes previously sent.
}
