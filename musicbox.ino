#include "Arduino.h"
#include "MusicBox.h"

MusicBox musicBox;
void setup(){
  musicBox.initialize(false);
}

void loop(){
  musicBox.loop();
}