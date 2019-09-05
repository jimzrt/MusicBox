#ifndef EEPROMUTILITY_H
#define EEPROMUTILITY_H

#include <Arduino.h>
#include <EEPROM.h>

#define CONFIG_OFFSET 0
#define SETTING_OFFSET 10
#define TAG_OFFSET 20

#define NO_TAG 255

typedef enum : uint8_t
{
    FREE = 55,
    USED = 56
} TagType;

typedef struct
{
    uint8_t id;
    uint8_t lowestFree;
    uint8_t highestFree;
    uint8_t folderCount;
    uint8_t freeCount;
} MusicBox_config;

typedef struct
{
    TagType tagType;
    uint8_t prev;
    uint8_t next;
} MusicBox_tag;

bool EEPROM_updateTag(uint8_t index, TagType tagType, MusicBox_config &config);
uint8_t EEPROM_getPreviousFree(uint8_t index);
uint8_t EEPROM_getNextFree(uint8_t index);
int EEPROM_writeConfig(const MusicBox_config &value);
int EEPROM_getConfig(MusicBox_config &value);
void EEPROM_initialize(const MusicBox_config &value);
int EEPROM_writeTag(uint8_t index, TagType tagType, uint8_t prev, uint8_t next);
int EEPROM_getTag(uint8_t index, MusicBox_tag &tag);

#endif