#include "NFCHandler.h"

NFCHandler::NFCHandler(const uint8_t pin1, const uint8_t pin2)
{
    this->nfc = new Adafruit_PN532(pin1, pin2);
}

NFCHandler::~NFCHandler()
{
    free(this->nfc);
}

bool NFCHandler::initialize()
{
    nfc->begin();
    uint32_t versiondata = nfc->getFirmwareVersion();
    if (versiondata)
    {
        // configure board to read RFID tags
        nfc->SAMConfig();
        return true;
    }
    return false;
}