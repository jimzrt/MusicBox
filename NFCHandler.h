#ifndef NFCHANDLER_H
#define NFCHANDLER_H

#include <Arduino.h>
#include "Adafruit_PN532.h"

typedef enum : uint8_t
{
    TAG,
    MIFARE,
    NO_CARD
} CardType;

class NFCHandler
{
private:
    Adafruit_PN532 nfc;
    uint8_t buffer[7];
    uint8_t bufferLength;

public:
    NFCHandler(const uint8_t pin1, const uint8_t pin2);
    ~NFCHandler();
    bool initialize();
    bool isCardPresent();
    CardType getCardType();
};
#endif