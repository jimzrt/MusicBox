#include "NFCHandler.h"

NFCHandler::NFCHandler(const uint8_t pin1, const uint8_t pin2) : nfc(pin1, pin2), buffer {0,0,0,0,0,0,0}, bufferLength(0)
{
}

NFCHandler::~NFCHandler()
{
}

bool NFCHandler::initialize()
{
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (versiondata)
    {
        // configure board to read RFID tags
        nfc.SAMConfig();
        return true;
    }
    return false;
}

bool NFCHandler::isCardPresent()
{
   bool cardPresent = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, buffer, &bufferLength, 150);
   if(!cardPresent){
       bufferLength = 0;
   }
   return cardPresent;
}

CardType NFCHandler::getCardType()
{
    if(bufferLength == 4){
        return TAG;
    } else if(bufferLength == 7){
        return MIFARE;
    } else {
        return NO_CARD;
    }
}