#ifndef NFCHANDLER_H
#define NFCHANDLER_H

#include <Arduino.h>
#include "Adafruit_PN532.h"

class NFCHandler
{
private:
    Adafruit_PN532 nfc;

public:
    NFCHandler(const uint8_t pin1, const uint8_t pin2);
    ~NFCHandler();
    bool initialize();
};
#endif