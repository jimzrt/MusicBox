#include "MusicBox.h"

MusicBox::MusicBox(uint8_t id) : id(id), musicHandler(MP3_PIN_RX, MP3_PIN_TX), nfcHandler(NFC_PIN_1, NFC_PIN_2), buttonHandler(BTN1_PIN, BTN2_PIN, BTN3_PIN), startTime(millis()), state(IDLE)
{
}

MusicBox::~MusicBox()
{
}

void MusicBox::initialize()
{

  // init serial
  Serial.begin(115200);
  DEBUG_PRINTLN(F("Initializing..."));

  // init buttons
  DEBUG_PRINTLN(F("Initializing Buttons..."));
  buttonHandler.initialize();

  // init LED Ring
  // todo

  // init MiniMp3Player
  DEBUG_PRINTLN(F("Initializing MP3 Player..."));
  musicHandler.initialize();

  // init nfc
  DEBUG_PRINTLN(F("Initializing NFC Reader..."));
  if (!nfcHandler.initialize())
  {
    DEBUG_PRINTLN(F("Didn't find PN53x board"));
    while (1)
      ; // halt
  }

  // initialize MusicBox
  DEBUG_PRINTLN(F("Initializing MusicBox..."));

  musicHandler.playVoiceTrackAndWait(AUDIO_HELLO);
  initializeMusicBox();
  musicHandler.playVoiceTrackAndWait(AUDIO_INITIALIZED);
}

void MusicBox::initializeMusicBox()
{
  EEPROM_getConfig(config);

  uint8_t folderCount = musicHandler.getFolderCount();
  DEBUG_PRINT("Foldertrack count");
  DEBUG_PRINTLN(folderCount);
  if (config.id == this->id)
  {
    if (folderCount == config.folderCount)
    {
      // all good
      DEBUG_PRINTLN("all good");
      return;
    }
    else
    {
      //same id, different folder count
      DEBUG_PRINTLN("Warining: same id, different folder count");
    }
  }
  else
  {
    // different id -> initialize musicBox
    DEBUG_PRINTLN("different id -> initialize musicBox");

    config.id = this->id;
    config.lowestFree = 1;
    config.highestFree = folderCount - 1;
    config.folderCount = folderCount;
    config.freeCount = folderCount;
    EEPROM_initialize(config);
    DEBUG_PRINTLN("done");
  }
}

void MusicBox::loop()
{

  switch (state)
  {
  case PLAYING:
    handlePlaying();
    break;
  case IDLE:
    handleIdle();
    break;
  }

  delay(100);
}

uint8_t numberOfTracksInFolder = 0;

bool MusicBox::handleButtons()
{
  // volume
  if (buttonHandler.pressedNextHold())
  {
    musicHandler.increaseVolume();
    return true;
  }
  else if (buttonHandler.pressedPreviousHold())
  {
    musicHandler.decreaseVolume();
    return true;
  }

  // next/previous track
  else if (buttonHandler.pressedNext())
  {

    if (currentTag.track == numberOfTracksInFolder)
    {
      currentTag.track = 1;
    }
    else
    {
      currentTag.track += 1;
    }
    nfcHandler.updateMusicTagTrack(currentTag);
    musicHandler.playTrackName(currentTag.track);
    musicHandler.playFolderTrack(currentTag.folder, currentTag.track);
    return true;
  }
  else if (buttonHandler.pressedPrevious())
  {
    if (currentTag.track == 1)
      return true;
    currentTag.track -= 1;
    nfcHandler.updateMusicTagTrack(currentTag);
    musicHandler.playTrackName(currentTag.track);
    musicHandler.playFolderTrack(currentTag.folder, currentTag.track);
    return true;
  }

  // delete tag
  else if (buttonHandler.pressedConfirmHold())
  {
    musicHandler.playVoiceTrackAndWait(AUDIO_TAG_DELETE);
    state = IDLE;
    bool confirm = false;
    while (nfcHandler.getCardType() == TAG)
    {
      buttonHandler.readButtons();
      confirm = buttonHandler.pressedConfirm();
      if (confirm)
      {
        break;
      }
      delay(50);
    }
    if (!confirm)
    {
      DEBUG_PRINTLN("tag not present anymore! goodbye");
      musicHandler.playVoiceTrackAndWait(AUDIO_TAG_REMOVED);
      return true;
    }
    uint8_t folder = currentTag.folder;
    if (!nfcHandler.deleteTag(currentTag))
    {
      DEBUG_PRINTLN("error deleting tag!");
      musicHandler.playVoiceTrackAndWait(AUDIO_TAG_DELETE_FAIL);
      return true;
    }
    if (folder < config.lowestFree)
    {
      config.lowestFree = folder;
    }
    EEPROM_updateTag(folder, FREE, config);
    musicHandler.playVoiceTrackAndWait(AUDIO_TAG_DELETED);
    return true;
  }
  return false;
}

uint16_t cycles = 0;
void MusicBox::handlePlaying()
{
  cycles++;
  if (cycles % 10 == 0)
  {
    CardType cardType = nfcHandler.getCardType();
    if (cardType != TAG)
    {
      DEBUG_PRINTLN("is playing and tag is not present, stopping!");
      musicHandler.playVoiceTrackAndWait(AUDIO_TAG_REMOVED);
      state = IDLE;
      return;
    }
  }

  buttonHandler.readButtons();
  musicHandler.update();
  if (handleButtons())
  {
    return;
  }

  if (musicHandler.isPlayFinished())
  {
    DEBUG_PRINTLN("next track!");
    if (currentTag.track == numberOfTracksInFolder)
    {
      currentTag.track = 1;
    }
    else
    {
      currentTag.track += 1;
    }
    nfcHandler.updateMusicTagTrack(currentTag);
    musicHandler.playTrackName(currentTag.track);
    musicHandler.playFolderTrack(currentTag.folder, currentTag.track);
  }
}

void MusicBox::handleIdle()
{
  CardType cardType = nfcHandler.getCardType();
  if (cardType == TAG)
  {

    MusicTag tag;
    if (!nfcHandler.isMusicTag())
    {
      musicHandler.playVoiceTrackAndWait(AUDIO_TAG_EMPTY);
      DEBUG_PRINTLN("new tag found!");
      uint8_t currentFolder = config.lowestFree;
      DEBUG_PRINT("playing: ");
      DEBUG_PRINTLN(currentFolder);
      musicHandler.playFolderName(currentFolder);
      musicHandler.playFolderTrack(currentFolder, 1);
      bool confirm = false;

      while (nfcHandler.getCardType() == TAG)
      {
        buttonHandler.readButtons();
        confirm = buttonHandler.pressedConfirm();
        if (confirm)
        {
          break;
        }
        if (buttonHandler.pressedNext())
        {
          currentFolder = EEPROM_getNextFree(currentFolder);
          musicHandler.playFolderName(currentFolder);
          musicHandler.playFolderTrack(currentFolder, 1);
          DEBUG_PRINT("playing: ");
          DEBUG_PRINTLN(currentFolder);
        }
        else if (buttonHandler.pressedPrevious())
        {
          currentFolder = EEPROM_getPreviousFree(currentFolder);
          musicHandler.playFolderName(currentFolder);
          musicHandler.playFolderTrack(currentFolder, 1);
          DEBUG_PRINT("playing: ");
          DEBUG_PRINTLN(currentFolder);
        }
        delay(50);
      }
      musicHandler.stop();
      if (!confirm)
      {
        musicHandler.playVoiceTrackAndWait(AUDIO_TAG_REMOVED);
        DEBUG_PRINTLN("tag not present anymore! goodbye");
        return;
      }

      DEBUG_PRINT("you chose ");
      DEBUG_PRINTLN(currentFolder);
      if (!nfcHandler.createMusicTag(id, currentFolder, tag))
      {
        musicHandler.playVoiceTrackAndWait(AUDIO_TAG_SAVED_FAIL);
        DEBUG_PRINTLN("could not write tag!");
        return;
      }
      musicHandler.playVoiceTrackAndWait(AUDIO_TAG_SAVED);
      EEPROM_updateTag(currentFolder, USED, config);
    }
    else
    {
      DEBUG_PRINTLN("found musictag!");
      if (!nfcHandler.readTag(tag))
      {
        DEBUG_PRINTLN("could not read tag!");
        return;
      }
    }

    if (tag.id != this->id)
    {
      DEBUG_PRINTLN("id does not match!");

      // todo test
      nfcHandler.deleteTag(tag);
      return;
    }

    numberOfTracksInFolder = musicHandler.getFolderTrackCount(tag.folder);
    musicHandler.playFolderName(tag.folder);
    musicHandler.playTrackName(tag.track);
    musicHandler.playFolderTrack(tag.folder, tag.track);
    state = PLAYING;
    currentTag = tag;
    return;
  }
}
