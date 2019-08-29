#include "MusicHandler.h"

void MUSIC_initialize(DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3){
    mp3->stop();
    mp3->begin();
    delay(1000);
}

uint8_t MUSIC_getFolderCount(DFMiniMp3<SoftwareSerial, Mp3Notify> *mp3){
    return ((uint8_t) mp3->getTotalFolderCount()) - 1;
}