#ifndef MusicBox_h
#define MusicBox_h


// Buttons
#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4
#define MP3_PIN_RX 8
#define MP3_PIN_TX 9


#include "Arduino.h"
#include "JC_Button.h"
#include "MusicHandler.h"
#include "NFCHandler.h"
#include "EEPROMAnything.h"


enum State
{
  PLAYING,
  IDLE
};

class MusicBox
{
public:
  MusicBox();
  ~MusicBox();
  void initialize(int id);
  void loop();

private:
  int id;
  State state;
  MusicBox_config config;
  unsigned long startTime;
  MusicHandler musicHandler;
  NFCHandler nfcHandler;
  void tick();
  bool cardPresent();
  void checkCard();
  void checkButtons();
  void checkTimeout();
  void initializeMusicBox();
};

#endif