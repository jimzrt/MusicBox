#ifndef MUSICHANDLER_H
#define MUSICHANDLER_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>
#include "MP3Notify.h"

class MusicHandler
{
private:
    SoftwareSerial softwareSerial;
    DFMiniMp3<SoftwareSerial, Mp3Notify> mp3;

public:
    MusicHandler(uint8_t receivePin, uint8_t transmitPin);
    ~MusicHandler();
    void initialize();
    uint8_t getFolderCount();
    void stop();
    void play();
    void playFolderTrack(uint8_t folder, uint8_t track);
    void increaseVolume();
    void decreaseVolume();
    uint8_t getFolderTrackCount(uint8_t folder);
};

#endif
