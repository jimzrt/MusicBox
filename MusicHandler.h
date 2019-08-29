#ifndef MUSICHANDLER_H
#define MUSICHANDLER_H

#include <Arduino.h> 
#include "DFMiniMp3.h"
#include "SoftwareSerial.h"
#include "MP3Notify.h"


void MUSIC_initialize(DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3);
uint8_t MUSIC_getFolderCount(DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3);
#endif
