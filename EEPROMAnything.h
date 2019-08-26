#ifndef EEPROMANYTHING_H
#define EEPROMANYTHING_H

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

#define CONFIG_OFFSET 0

typedef struct 
{
    int id;
    byte head;
    byte lowestFree;
    byte highestFree;
    byte folderCount;
} musicBox_config;

int EEPROM_writeConfig(const musicBox_config& value);
int EEPROM_getConfig(musicBox_config& value);

#endif //EEPROMANYTHING_H