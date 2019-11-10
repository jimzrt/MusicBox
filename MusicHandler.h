#ifndef MUSICHANDLER_H
#define MUSICHANDLER_H

#include <Arduino.h>
#include "libs/DFMiniMp3.h"
#include <SoftwareSerial.h>

#define AUDIO_HELLO 256
#define AUDIO_INITIALIZED 257
#define AUDIO_INITIALIZE 258
#define AUDIO_TAG_EMPTY 259
#define AUDIO_TAG_REMOVED 260
#define AUDIO_TAG 261
#define AUDIO_TAG_DELETED 262
#define AUDIO_TAG_DELETE 263
#define AUDIO_TAG_DELETE_FAIL 264
#define AUDIO_TAG_SAVED_FAIL 265
#define AUDIO_TAG_SAVED 266
#define AUDIO_TAG_FOLDER 267
#define AUDIO_TAG_TRACK 268

class MusicHandler
{
private:
    SoftwareSerial softwareSerial;
    DFMiniMp3<SoftwareSerial, MusicHandler> mp3;
    bool playFinished;
    // events
    void OnPlayFinished(uint16_t replyArg);
    void OnError(uint16_t replyArg);
    friend class DFMiniMp3<SoftwareSerial, MusicHandler>;

public:
    MusicHandler(uint8_t receivePin, uint8_t transmitPin);
    ~MusicHandler();
    void initialize();
    uint8_t getFolderCount();
    void stop();
    void play();
    void update();
    void playFolderTrack(uint8_t folder, uint8_t track);
    void playVoiceTrack(uint16_t track);
    void playVoiceTrackAndWait(uint16_t track);
    void playFolderName(uint8_t folder);
    void playTrackName(uint16_t track);
    void increaseVolume();
    void decreaseVolume();
    uint8_t getFolderTrackCount(uint8_t folder);

    bool isPlayFinished();
};

#endif
