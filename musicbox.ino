/*
Libraries used:
- Adafruit_PN532
- DFPlayer_Mini_Mp3_by_Makuna,
- JC_Button v2.1.1


  */
#include "Arduino.h"
#include "MusicBox.h"

MusicBox musicBox;
void setup(){
  musicBox.initialize(false);
}

void loop(){
  musicBox.loop();
}