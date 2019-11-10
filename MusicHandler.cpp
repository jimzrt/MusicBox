#include "MusicHandler.h"

MusicHandler::MusicHandler(uint8_t receivePin, uint8_t transmitPin) : softwareSerial(receivePin, transmitPin), mp3(softwareSerial, *this), playFinished(true)
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
    mp3.setVolume(20);
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

void MusicHandler::update()
{
    mp3.loop();
}

void MusicHandler::playFolderTrack(uint8_t folder, uint8_t track)
{
    mp3.playFolderTrack(folder, track);
    playFinished = false;
}

void MusicHandler::playVoiceTrack(uint16_t track)
{
    mp3.playMp3FolderTrack(track);
    playFinished = false;
}
void MusicHandler::playFolderName(uint8_t folder)
{
    playVoiceTrackAndWait(AUDIO_TAG_FOLDER);
    playVoiceTrackAndWait(folder);
}

void MusicHandler::playTrackName(uint16_t track)
{
    playVoiceTrackAndWait(AUDIO_TAG_TRACK);
    playVoiceTrackAndWait(track);
}

void MusicHandler::playVoiceTrackAndWait(uint16_t track)
{
    playVoiceTrack(track);
    while (!playFinished)
    {
        mp3.loop();
        delay(50);
    }
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
    return mp3.getFolderTrackCount(folder);
}
void MusicHandler::OnPlayFinished(uint16_t replyArg)
{
    playFinished = true;
}
void MusicHandler::OnError(uint16_t replyArg)
{
}

bool MusicHandler::isPlayFinished()
{
    return playFinished;
}