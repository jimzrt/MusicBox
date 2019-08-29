#include "MusicBox.h"


MusicBox::MusicBox(int id) : id(id), musicHandler(MP3_PIN_RX, MP3_PIN_TX), nfcHandler(NFC_PIN_1, NFC_PIN_2), buttonHandler(BTN1_PIN, BTN2_PIN, BTN3_PIN), startTime(millis())
{
}

MusicBox::~MusicBox()
{
}

void MusicBox::initialize()
{
 // this->id = id;

  // init serial
  Serial.begin(115200);
  Serial.println(F("Initializing..."));

  // init buttons
  Serial.println(F("Initializing Buttons..."));
  buttonHandler.initialize();

  // init LED Ring
  // todo

  // init MiniMp3Player
  Serial.println(F("Initializing MP3 Player..."));
  musicHandler.initialize();

  // init nfc
  Serial.println(F("Initializing NFC Reader..."));
  if (!nfcHandler.initialize())
  {
    Serial.println(F("Didn't find PN53x board"));
    while (1)
      ; // halt
  }

  // initialize MusicBox
   Serial.println(F("Initializing MusicBox..."));
  initializeMusicBox();
}

void MusicBox::loop()
{
  //  delay(500);
  // Serial.println(config.id);
}

void MusicBox::initializeMusicBox()
{
  EEPROM_getConfig(config);

  uint8_t folderCount = musicHandler.getFolderCount();
  if (config.id == this->id)
  {
    if (folderCount == config.folderCount)
    {
      // all good
      Serial.println("all good");
      return;
    }
    else
    {
      //same id, different folder count
      Serial.println("Warining: same id, different folder count");
    }
  }
  else
  {
    // different id -> initialize musicBox
    Serial.println("different id -> initialize musicBox");

    config.id = this->id;
    config.head = 0;
    config.lowestFree = 0;
    config.highestFree = folderCount - 1;
    config.folderCount = folderCount;
    config.freeCount = folderCount;
    EEPROM_initialize(config);
    Serial.println("done");
  }
}