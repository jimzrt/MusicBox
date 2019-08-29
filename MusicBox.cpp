#include "MusicBox.h"


MusicBox::MusicBox() : musicHandler(MP3_PIN_RX, MP3_PIN_TX), nfcHandler(NFC_PIN_1, NFC_PIN_2), buttonHandler(BTN1_PIN, BTN2_PIN, BTN3_PIN), startTime(millis())
{
}

MusicBox::~MusicBox()
{
}

void MusicBox::initialize(int id)
{
  this->id = id;

  // init serial
  Serial.begin(115200);
  Serial.println(F("Lets go my friend"));

  // init buttons
  buttonHandler.initialize();

  // init LED Ring
  // todo

  // init MiniMp3Player
  musicHandler.initialize();

  // init nfc
  if (!nfcHandler.initialize())
  {
    Serial.println(F("Didn't find PN53x board"));
    while (1)
      ; // halt
  }

  // initialize MusicBox
  initializeMusicBox();
}

void MusicBox::loop()
{
  //  delay(500);
  // Serial.println(config.id);
}

void MusicBox::initializeMusicBox()
{
  uint8_t folderCount = musicHandler.getFolderCount();
  EEPROM_getConfig(config);
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
    Serial.println("different id -> initialize musicBox");
    // different id -> initialize musicBox

    config.id = this->id;
    config.head = 0;
    config.lowestFree = 0;
    config.highestFree = folderCount - 1;
    config.folderCount = folderCount;
    config.freeCount = folderCount;
    EEPROM_initialize(config);
    Serial.print("done");
  }
  Serial.println(config.id);
}