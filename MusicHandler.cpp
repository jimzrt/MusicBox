#include "MusicHandler.h"

MusicHandler::MusicHandler(uint8_t receivePin, uint8_t transmitPin) : softwareSerial(receivePin, transmitPin), playFinished(true)
{
}

MusicHandler::~MusicHandler()
{
}

void MusicHandler::initialize()
{
    //softwareSerial.end();
    softwareSerial.begin(9600);
    //mp3.stop();
    if(!mp3.begin(softwareSerial, false, false)){
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
        while(true);
    }
    Serial.println(F("DFPlayer Mini online."));
    delay(4000);
  
     mp3.setTimeOut(5000); //Set serial communictaion time out 500ms
    // mp3.stop();
    // mp3.begin();
    // delay(1000);
     mp3.volume(20);
}

uint8_t MusicHandler::getFolderCount()
{
    // -2 because of mp3 folder and ad folder
    return ((uint8_t)mp3.readFolderCounts()) - 2;
}

void MusicHandler::stop()
{
    mp3.stop();
}

void MusicHandler::loop()
{
   // mp3.loop();
//    if(mp3.available()){
//        if(mp3.readType() == DFPlayerPlayFinished){
//            playFinished = true;
//             Serial.println("pd");
//        }
//    }
}

unsigned long start = 0;

void MusicHandler::playFolderTrack(uint8_t folder, uint8_t track)
{
   // mp3.stop();
  //  mp3.drainResponses();
    mp3.playFolder(folder, track);
  //      delay(100);
    start = millis();
    playFinished = false;
  //  delay(10);

    
}

void MusicHandler::playVoiceTrack(uint16_t track)
{
   // mp3.stop();
 //   mp3.drainResponses();
    mp3.playMp3Folder(track);
     start = millis();
  //  delay(100);
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
    mp3.volumeUp();
}
void MusicHandler::decreaseVolume()
{
    mp3.volumeDown();
}
uint8_t MusicHandler::getFolderTrackCount(uint8_t folder)
{
    return mp3.readFileCountsInFolder(folder);
}
// void MusicHandler::OnPlayFinished(uint16_t replyArg)
// {
//    // delay(10);
//     playFinished = true;
//         Serial.println("pd");

// }
// void MusicHandler::OnError(uint16_t replyArg)
// {
//     Serial.println("ERRORRR!!!");
//     playFinished = true;
// }

bool playStarted = false;
bool MusicHandler::isPlayFinished()
{

    //Serial.println(mp3.readState(), HEX);
    uint8_t state = mp3.readState();
    Serial.println(state, HEX);
    if(state == 0 && (playStarted || start + 500 < millis())){
        playStarted = false;
        return true;
    } else if(!playStarted && state == 1){
        playStarted = true;
        return false;
    } else {
        return false;
    }

}