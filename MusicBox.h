/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef MusicBox_h
#define MusicBox_h

#include "Arduino.h"
#include "JC_Button.h"
#include "EEPROMAnything.h"

// Buttons
#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4

enum State
{
  PLAYING,
  IDLE
};

class MusicBox
{
public:
  void initialize(int id);
  void loop();

private:
  int id;
  State state;
  unsigned long startTime;
  uint8_t nextFreeFolder;
  // musicBox_config *config;
  void tick();
  bool cardPresent();
  void checkCard();
  void checkButtons();
  void checkTimeout();
  void initializeMusicBox();
};

#endif