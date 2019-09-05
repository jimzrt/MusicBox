#ifndef MusicBox_h
#define MusicBox_h

// hardware pins
#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4
#define NFC_PIN_1 A2
#define NFC_PIN_2 A3
#define MP3_PIN_RX 8
#define MP3_PIN_TX 9

#include <Arduino.h>
#include "MusicHandler.h"
#include "NFCHandler.h"
#include "ButtonHandler.h"
#include "EEPROMUtility.h"

enum State
{
  PLAYING,
  IDLE
};

class MusicBox
{
public:
  MusicBox(uint8_t id);
  ~MusicBox();
  void initialize();
  void loop();

private:
  uint8_t id;
  State state;
  MusicTag currentTag;
  MusicBox_config config;
  unsigned long startTime;
  MusicHandler musicHandler;
  NFCHandler nfcHandler;
  ButtonHandler buttonHandler;
  void tick();
  void initializeMusicBox();
  void handlePlaying();
  void handleIdle();
};

#endif