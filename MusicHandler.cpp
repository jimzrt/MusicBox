#include "MusicHandler.h"

MusicHandler::MusicHandler(uint8_t receivePin, uint8_t transmitPin) : softwareSerial(receivePin, transmitPin), mp3(softwareSerial)
{
}

MusicHandler::~MusicHandler()
{
}

void MusicHandler::initialize()
{
    mp3.stop();
    mp3.begin();
    delay(1000);
}

uint8_t MusicHandler::getFolderCount()
{
    // -2 because of mp3 folder and ad folder
    return ((uint8_t)mp3.getTotalFolderCount()) - 2;
}

void MusicHandler::stop()
{
    mp3.stop();
}

void MusicHandler::playFolderTrack(uint8_t folder, uint8_t track)
{
    mp3.playFolderTrack(folder, track);
}

void MusicHandler::increaseVolume()
{
    mp3.increaseVolume();
}
void MusicHandler::decreaseVolume()
{
    mp3.decreaseVolume();
}
uint8_t MusicHandler::getFolderTrackCount(uint8_t folder)
{
    // todo: not working?
    //return mp3.getFolderTrackCount(folder) - 1;
    return 10;
}
