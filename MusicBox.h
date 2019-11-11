#ifndef MusicBox_h
#define MusicBox_h

// debug flag
#define DEBUG 1

// hardware pins
#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4
#define NFC_PIN_1 A2
#define NFC_PIN_2 A3
#define MP3_PIN_RX 8
#define MP3_PIN_TX 9
#define LEDPIN         6
#define NUMPIXELS      24

#if DEBUG == 1
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#include <Arduino.h>
#include "MusicHandler.h"
//#include "MusicHandler.cpp"
#include "NFCHandler.h"
#include "ButtonHandler.h"
#include "LEDHandler.h"
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
  void service_delay(uint16_t sleep);
  void service();

private:
  uint8_t id;
  State state;
  MusicTag currentTag;
  MusicBox_config config;
  unsigned long startTime;
  MusicHandler musicHandler;
  NFCHandler nfcHandler;
  ButtonHandler buttonHandler;
  LEDHandler ledHandler;
  void tick();
  void initializeMusicBox();
  bool handleButtons();
  void handlePlaying();
  void handleIdle();
};

#endif