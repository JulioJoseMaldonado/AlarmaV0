#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "ReadRC522.h"

#define RST_PIN	9    //Pin 9 para el reset del RC522
#define SS_PIN	10   //Pin 10 para el SS (SDA) del RC522

//Tag value valid for the system.
byte key[] = {0xAA, 0xDD, 0x4B, 0xD3};

//Indicates to the module the  reset pin and pin select slave.
MFRC522 mfrc522(10, 9);

//Initalize de SPI protcol and the rc522.
void SetupRC522()
{
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
}
//Indicate that is tags near of antena and it's valid.
//True when this occurs. False when not.
bool ReadTag()
{
  bool valid = false;
  if ( mfrc522.PICC_IsNewCardPresent()) //Theres a tags near a RC522.
  {
    if ( mfrc522.PICC_ReadCardSerial()) //Warns that the tag could be read.
    {
      if (sizeof(key) == mfrc522.uid.size) //Compare the size of the values the current tag and the tag valid for the system.
      {
        valid = true;
        //Compare byte by byte to see if they are the same.
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          if(!(key[i] == mfrc522.uid.uidByte[i]))
          {
            //The tag present is not valid for the system.
            valid = false;
            break;
          }
        }
        mfrc522.PICC_HaltA(); //End the current lecture.
      }
    }
  }
  return (valid);
}
