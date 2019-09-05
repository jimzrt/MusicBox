#include "NFCHandler.h"

NFCHandler::NFCHandler(const uint8_t pin1, const uint8_t pin2) : nfc(pin1, pin2), buffer{0, 0, 0, 0, 0, 0, 0}, bufferLength(0)
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

// bool NFCHandler::isCardPresent()
// {
//    bool cardPresent = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, buffer, &bufferLength, 150);
//    if(!cardPresent){
//        bufferLength = 0;
//    }
//    return cardPresent;
// }

CardType NFCHandler::getCardType()
{
    bufferLength = 0;
    nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, buffer, &bufferLength, 150);
    if (bufferLength == 4)
    {
        return MIFARE;
    }
    else if (bufferLength == 7)
    {
        return TAG;
    }
    else
    {
        return NO_CARD;
    }
}

bool NFCHandler::isMusicTag()
{
    uint8_t pageBuffer[4] = {0};
    nfc.ntag2xx_ReadPage(4, pageBuffer);

    for (int i = 0; i < 4; i++)
    {
        if (magic[i] != pageBuffer[i])
        {
            return false;
        }
    }
    return true;
}

bool NFCHandler::readTag(MusicTag &tag)
{
    uint8_t pageBuffer[4] = {0};
    if (!nfc.ntag2xx_ReadPage(5, pageBuffer))
        return false;
    tag.id = pageBuffer[0];

    if (!nfc.ntag2xx_ReadPage(6, pageBuffer))
        return false;
    tag.folder = pageBuffer[0];

    if (!nfc.ntag2xx_ReadPage(7, pageBuffer))
        return false;
    tag.track = pageBuffer[0];

    return true;
}

bool NFCHandler::createMusicTag(uint8_t id, uint8_t folder, MusicTag &tag)
{
    tag.id = id;
    tag.folder = folder;
    tag.track = 1;
    return writeMusicTag(tag);
}

bool NFCHandler::writeMusicTag(MusicTag &tag)
{

    uint8_t pageBuffer[4] = {0};

    if (!(nfc.ntag2xx_WritePage(4, magic)))
        return false;
    // DONE!

    //id

    //const char * idCopy = tag.id;
    //memcpy(pageBuffer, idCopy, 4);
    pageBuffer[0] = tag.id;
    if (!(nfc.ntag2xx_WritePage(5, pageBuffer)))
        return false;
    //return true;

    //idCopy += 4;
    //memcpy(pageBuffer, idCopy, 4);
    pageBuffer[0] = tag.folder;
    if (!(nfc.ntag2xx_WritePage(6, pageBuffer)))
        return false;

    pageBuffer[0] = tag.track;
    if (!(nfc.ntag2xx_WritePage(7, pageBuffer)))
        return false;

    return true;
}

bool NFCHandler::updateMusicTagTrack(MusicTag &tag)
{
    uint8_t pageBuffer[4] = {0};
    pageBuffer[0] = tag.track;
    if (!(nfc.ntag2xx_WritePage(7, pageBuffer)))
        return false;

    return true;
}

bool NFCHandler::deleteTag(MusicTag &tag)
{
    uint8_t pageBuffer[4] = {0};
    if (!(nfc.ntag2xx_WritePage(4, pageBuffer)))
        return false;

    return true;
}