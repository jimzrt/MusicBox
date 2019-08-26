#include "EEPROMAnything.h"

int EEPROM_writeConfig(const musicBox_config& value)
{
    int ee = CONFIG_OFFSET;
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

int EEPROM_getConfig(musicBox_config& value)
{
    int ee = CONFIG_OFFSET;
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}