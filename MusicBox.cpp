#include "Arduino.h"
#include "MusicBox.h"


 SoftwareSerial mySoftwareSerial(8, 9);
 DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

void MusicBox::initialize(bool initial){
    

    // init serial
    Serial.begin(115200);
    Serial.println(F("Lets go my friend"));

    // init buttons
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT_PULLUP);
    pinMode(BTN3_PIN, INPUT_PULLUP);    

    // init LED Ring
    // todo

    // init MiniMp3Player
    
    //mp3 =&_mp3;
    mp3.stop();
    mp3.begin();

    uint16_t volume = mp3.getVolume();
    Serial.print("volume ");
    Serial.println(volume);
    mp3.setVolume(10);
    
    uint16_t count = mp3.getTotalTrackCount();
    Serial.print("files ");
    Serial.println(count);
    
    Serial.println("starting...");
   
    mp3.playFolderTrack(1, 1);
//mp3.playRandomTrackFromAll();
   // serial = &mySoftwareSerial;
   // serial->begin(9600);
//    mySoftwareSerial.begin(9600);
//     if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.
//         Serial.println(F("Unable to begin:"));
//         Serial.println(F("1.Please recheck the connection!"));
//         Serial.println(F("2.Please insert the SD card!"));
//         while(true);
//     }
//     myDFPlayer.volume(10);
//     Serial.println(F("DFPlayer Mini online."));
//     myDFPlayer.loopFolder(2);

//         for(int i = 0; i < 100; i++){
//         Serial.println(myDFPlayer.readState()); //read mp3 state
//     Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
//     delay(1000);
//     }


}

void MusicBox::loop(){
    //  Serial.println(myDFPlayer.readState()); //read mp3 state
    //  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
    // delay(3000);
 //   myDFPlayer.next();
Serial.println(mp3.getCurrentTrack());
// mp3.playRandomTrackFromAll();
  delay(3000);

}