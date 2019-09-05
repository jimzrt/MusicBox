#ifndef NFCHANDLER_H
#define NFCHANDLER_H

#include <Arduino.h>
#include <Adafruit_PN532.h>

typedef enum : uint8_t
{
    TAG,
    MIFARE,
    NO_CARD
} CardType;

typedef struct
{
    uint8_t id;
    uint8_t folder;
    uint8_t track;
} MusicTag;

class NFCHandler
{
private:
    Adafruit_PN532 nfc;
    uint8_t buffer[7];
    uint8_t bufferLength;
    uint8_t magic[4] = {1, 3, 3, 7};
    bool writeMusicTag(MusicTag &tag);

public:
    NFCHandler(const uint8_t pin1, const uint8_t pin2);
    ~NFCHandler();
    bool initialize();
    // bool isCardPresent();
    CardType getCardType();
    bool readTag(MusicTag &tag);
    bool isMusicTag();
    bool createMusicTag(uint8_t id, uint8_t folder, MusicTag &tag);
    bool updateMusicTagTrack(MusicTag &tag);
    bool deleteTag(MusicTag &tag);
};
#endif