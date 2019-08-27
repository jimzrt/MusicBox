#ifndef EEPROMANYTHING_H
#define EEPROMANYTHING_H

#include <EEPROM.h>
#include <Arduino.h> // for type definitions

#define CONFIG_OFFSET 0
#define SETTING_OFFSET 10
#define TAG_OFFSET 20

typedef enum : uint8_t
{
    FREE = 55,
    USED = 56
} TagType;

typedef struct
{
    int id;
    uint8_t head;
    uint8_t lowestFree;
    uint8_t highestFree;
    uint8_t folderCount;
} musicBox_config;

typedef struct
{
    TagType tagType;
    uint8_t prev;
    uint8_t next;
} musicBox_tag;

uint8_t EEPROM_getPreviousFree(uint8_t index);
uint8_t EEPROM_getNextFree(uint8_t index);
int EEPROM_writeConfig(const musicBox_config &value);
int EEPROM_getConfig(musicBox_config &value);
int EEPROM_initialize(const musicBox_config &value);
int EEPROM_writeTag(uint8_t index, TagType tagType, uint8_t prev, uint8_t next);
int EEPROM_getTag(uint8_t index, musicBox_tag &tag);
#endif //EEPROMANYTHING_H