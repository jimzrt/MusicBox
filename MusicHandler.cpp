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
    return ((uint8_t)mp3.getTotalFolderCount()) - 1;
}
