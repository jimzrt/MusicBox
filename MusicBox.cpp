#include "Arduino.h"
#include "MusicBox.h"
#include "Adafruit_PN532.h"
#include "DFMiniMp3.h"
#include "SoftwareSerial.h"
#include "MP3Notify.h"




// config
musicBox_config config;

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

    Serial.print("Tagsize: ");
  Serial.println(sizeof(musicBox_tag));

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
  // configure board to read RFID tags
  nfc.SAMConfig();

  // initialize MusicBox
  readSettings();
}

void MusicBox::loop()
{
//  delay(500);
 // Serial.println(config.id);
}

void MusicBox::readSettings()
{
   Serial.println(mp3.getTotalFolderCount());
       // config = new musicBox_config;
      EEPROM_getConfig(config);
      if(config.id == this->id){
          if( mp3.getTotalFolderCount() == config.folderCount){
            // all good
            Serial.println("all good");
            return;
          } else {
            //same id, different folder count
            Serial.println("same id, different folder count");
          }
      } else {
         Serial.println("different id -> initialize musicBox");
        // different id -> initialize musicBox
      
        config.id = this->id;
        config.head = 0;
        config.lowestFree=0;
        config.highestFree = mp3.getTotalFolderCount() -1;
        config.folderCount = mp3.getTotalFolderCount() ;
        //config = &c;
        int bytesWritten = EEPROM_writeConfig(config);
        Serial.print("bytes written: ");
        Serial.println(bytesWritten);
        
      }
      Serial.println(config.id);
      Serial.println(mp3.getTotalFolderCount());

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