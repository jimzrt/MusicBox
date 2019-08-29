#include "Arduino.h"
#include "MusicBox.h"
#include "Adafruit_PN532.h"
#include "DFMiniMp3.h"
#include "SoftwareSerial.h"
#include "MP3Notify.h"

// config
MusicBox_config config;

// buttons
Button btn1(BTN1_PIN);
Button btn2(BTN2_PIN);
Button btn3(BTN3_PIN);

// mp3 player
SoftwareSerial mySoftwareSerial(8, 9);
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

// nfc
Adafruit_PN532 nfc(A2, A3);

void MusicBox::initialize(int id)
{
  this->id = id;

  // init serial
  Serial.begin(115200);
  Serial.println(F("Lets go my friend"));

  // init buttons
  btn1.begin();
  btn2.begin();
  btn3.begin();

  // init LED Ring
  // todo

  // init MiniMp3Player
  mp3.stop();
  mp3.begin();
  delay(1000);

  // init nfc
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.println(F("Didn't find PN53x board"));
    while (1)
      ; // halt
  }
  // configure board to read RFID tags
  nfc.SAMConfig();

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
  uint8_t folderCount = ((uint8_t) mp3.getTotalFolderCount()) - 1;
  // config = new MusicBox_config;
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
    config.highestFree =folderCount - 1;
    config.folderCount = folderCount;
    config.freeCount = folderCount;
    EEPROM_initialize(config);
    Serial.print("done");
  }
  Serial.println(config.id);

  // todo
  // compare id in eeprom with defined id
  //  if match, compare folder count with actual count
  //    if match
  //      read header position
  //    else
  //      adjust pointer in eeprom
  //      read header position
  //  else
  //    reinitialize device and memory
}