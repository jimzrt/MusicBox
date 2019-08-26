#include "Arduino.h"
#include "MusicBox.h"
#include "Adafruit_PN532.h"
#include "DFMiniMp3.h"
#include "SoftwareSerial.h"
#include "MP3Notify.h"

#define MUSICBOX_ID 1





// buttons
Button btn1(BTN1_PIN);
Button btn2(BTN2_PIN);
Button btn3(BTN3_PIN);

// mp3 player
SoftwareSerial mySoftwareSerial(8, 9);
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

// nfc
Adafruit_PN532 nfc(A2, A3);

void MusicBox::initialize(bool initial)
{

  // musicBoxConfig config;
  // config.alarm = 10;
  // config.mode = 10;



  // init serial
  Serial.begin(115200);
  Serial.println(F("Lets go my friend"));

  // init buttons
  // pinMode(BTN1_PIN, INPUT_PULLUP);
  // pinMode(BTN2_PIN, INPUT_PULLUP);
  // pinMode(BTN3_PIN, INPUT_PULLUP);
  //btn1(BTN1_PIN);
  //btn1 = new Button(BTN1_PIN);
  btn1.begin();
  btn2.begin();
  btn3.begin();

  // init LED Ring
  // todo

  // init MiniMp3Player
  // mp3.reset();
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
  // // configure board to read RFID tags
  nfc.SAMConfig();

  // // initialize MusicBox
  readSettings();
}

void MusicBox::loop()
{
}

void MusicBox::readSettings()
{
      EEPROM_getConfig(*config);
      Serial.println(config->id);

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

  uint8_t availableFolders = mp3.getTotalFolderCount();
}