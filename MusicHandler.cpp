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

void MusicHandler::loop()
{
    mp3.loop();
}

void MusicHandler::playFolderTrack(uint8_t folder, uint8_t track)
{
    mp3.stop();
  //  mp3.drainResponses();
    mp3.playFolderTrack(folder, track);
    playFinished = false;
  //  delay(10);

    
}

void MusicHandler::playVoiceTrack(uint16_t track)
{
    mp3.stop();
 //   mp3.drainResponses();
    mp3.playMp3FolderTrack(track);
    playFinished = false;
 //   delay(10);


    
}
// void MusicHandler::playFolderName(uint8_t folder)
// {
//  //   playVoiceTrackAndWait<MusicHandler>(AUDIO_TAG_FOLDER, *this);
//  //   playVoiceTrackAndWait(folder);
// }

//void MusicHandler::playTrackName(uint16_t track)
//{
  //  playVoiceTrackAndWait(AUDIO_TAG_TRACK);
 //   playVoiceTrackAndWait(track);
//}

// template<typename SERVICE_CLASS> 
// void MusicHandler::playVoiceTrackAndWait(uint16_t track, SERVICE_CLASS& serviceClass)
// {
//     playVoiceTrack(track);
//     while (!playFinished)
//     {
//         mp3.loop();
//         delay(50);
//     }
// }

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
   // delay(10);
    playFinished = true;
        Serial.println("pd");

}
void MusicHandler::OnError(uint16_t replyArg)
{
    Serial.println("ERRORRR!!!");
    playFinished = true;
}

bool MusicHandler::isPlayFinished()
{
    return playFinished;
}