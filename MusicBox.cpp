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
  Serial.println(F("Initializing..."));

  // init buttons
  Serial.println(F("Initializing Buttons..."));
  buttonHandler.initialize();

  // init LED Ring
  // todo

  // init MiniMp3Player
  Serial.println(F("Initializing MP3 Player..."));
  musicHandler.initialize();

  // init nfc
  Serial.println(F("Initializing NFC Reader..."));
  if (!nfcHandler.initialize())
  {
    Serial.println(F("Didn't find PN53x board"));
    while (1)
      ; // halt
  }

  // initialize MusicBox
  Serial.println(F("Initializing MusicBox..."));
  initializeMusicBox();
}

void MusicBox::initializeMusicBox()
{
  EEPROM_getConfig(config);

  uint8_t folderCount = musicHandler.getFolderCount();
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
    // different id -> initialize musicBox
    Serial.println("different id -> initialize musicBox");

    config.id = this->id;
    config.lowestFree = 1;
    config.highestFree = folderCount - 1;
    config.folderCount = folderCount;
    config.freeCount = folderCount;
    EEPROM_initialize(config);
    Serial.println("done");
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

void MusicBox::handlePlaying()
{
  CardType cardType = nfcHandler.getCardType();
  if (cardType != TAG)
  {
    Serial.println("is playing and tag is not present!");
    musicHandler.stop();
    state = IDLE;
    return;
  }
  buttonHandler.readButtons();
  Serial.println("is playing and tag is present!");
  Serial.println("handle buttons");

  if (buttonHandler.pressedNextHold())
  {
    musicHandler.increaseVolume();
  }
  else if (buttonHandler.pressedPreviousHold())
  {
    musicHandler.decreaseVolume();
  }
  else if (buttonHandler.pressedNext())
  {

    Serial.print("current tracK: ");
    Serial.println(currentTag.track);
    Serial.println("track in folder: ");
    Serial.println(musicHandler.getFolderTrackCount(currentTag.folder));
    if (currentTag.track == musicHandler.getFolderTrackCount(currentTag.folder))
    {
      return;
    }
    currentTag.track += 1;
    nfcHandler.updateMusicTagTrack(currentTag);
    musicHandler.playFolderTrack(currentTag.folder, currentTag.track);
  }
  else if (buttonHandler.pressedPrevious())
  {
    if (currentTag.track == 1)
      return;
    currentTag.track -= 1;
    nfcHandler.updateMusicTagTrack(currentTag);
    musicHandler.playFolderTrack(currentTag.folder, currentTag.track);
  }
  else if (buttonHandler.pressedConfirmHold())
  {
    musicHandler.stop();
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
      Serial.println("tag not present anymore! goodbye");
      return;
    }
    uint8_t folder = currentTag.folder;
    if (!nfcHandler.deleteTag(currentTag))
    {
      Serial.println("error deleting tag!");
      return;
    }
    if (folder < config.lowestFree)
    {
      config.lowestFree = folder;
    }
    EEPROM_updateTag(folder, FREE, config);
  }
}

void MusicBox::handleIdle()
{
  CardType cardType = nfcHandler.getCardType();
  if (cardType == TAG)
  {
    Serial.println("is IDLE and tag is present!");
    MusicTag tag;
    if (!nfcHandler.isMusicTag())
    {
      Serial.println("new tag found!");
      uint8_t currentFolder = config.lowestFree;
      Serial.print("playing: ");
      Serial.println(currentFolder);
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
          musicHandler.playFolderTrack(currentFolder, 1);
          Serial.print("playing: ");
          Serial.println(currentFolder);
        }
        else if (buttonHandler.pressedPrevious())
        {
          currentFolder = EEPROM_getPreviousFree(currentFolder);
          musicHandler.playFolderTrack(currentFolder, 1);
          Serial.print("playing: ");
          Serial.println(currentFolder);
        }
        delay(50);
      }
      musicHandler.stop();
      if (!confirm)
      {
        Serial.println("tag not present anymore! goodbye");
        return;
      }

      Serial.print("you chose ");
      Serial.println(currentFolder);
      if (!nfcHandler.createMusicTag(id, currentFolder, tag))
      {
        Serial.println("could not write tag!");
        return;
      }

      EEPROM_updateTag(currentFolder, USED, config);
    }
    else
    {
      Serial.println("found musictag!");
      if (!nfcHandler.readTag(tag))
      {
        Serial.println("could not read tag!");
        return;
      }
    }

    if (tag.id != this->id)
    {
      Serial.println("id does not match!");

      // todo test
      nfcHandler.deleteTag(tag);
      return;
    }

    musicHandler.playFolderTrack(tag.folder, tag.track);
    state = PLAYING;
    currentTag = tag;
    return;
  }
  else
  {
    Serial.println("is IDLE and tag is not present!");
    return;
  }
}
