/*
Libraries used:
- Adafruit_PN532
- DFPlayer_Mini_Mp3_by_Makuna,
- JC_Button v2.1.1


  */
#include "Arduino.h"
#include "MusicBox.h"

#define MUSICBOX_ID 555

MusicBox musicBox;

void setup()
{
  musicBox.initialize(MUSICBOX_ID);
}

void loop()
{
  musicBox.loop();
}