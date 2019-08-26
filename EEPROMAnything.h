#ifndef EEPROMANYTHING_H
#define EEPROMANYTHING_H

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

#define CONFIG_OFFSET 0
#define SETTING_OFFSET 10

typedef enum:byte {
    FREE=0,
    USED=1
} TagType;

typedef struct 
{
    int id;
    byte head;
    byte lowestFree;
    byte highestFree;
    byte folderCount;
} musicBox_config;

typedef struct 
{
    TagType tagType;
    byte prev;
    byte next;
} musicBox_tag;


int EEPROM_writeConfig(const musicBox_config& value);
int EEPROM_getConfig(musicBox_config& value);

#endif //EEPROMANYTHING_H