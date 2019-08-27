#include "EEPROMAnything.h"

uint8_t EEPROM_getNextFree(uint8_t index)
{
    musicBox_tag tag;
    EEPROM_getTag(index, tag);
    return tag.prev;
}

uint8_t EEPROM_getPreviousFree(uint8_t index)
{
    musicBox_tag tag;
    EEPROM_getTag(index, tag);
    return tag.prev;
}

int EEPROM_writeTag(uint8_t index, TagType tagType, uint8_t prev, uint8_t next)
{
    musicBox_tag tag;
    tag.tagType = tagType;
    tag.prev = prev;
    tag.next = next;
    const byte *p = (const byte *)(const void *)&tag;
    int realIndex = TAG_OFFSET + (index * sizeof(tag));
    unsigned int i;
    for (i = 0; i < sizeof(tag); i++)
        EEPROM.write(realIndex++, *p++);
    return i;
}

int EEPROM_getTag(uint8_t index, musicBox_tag &tag)
{
    byte *p = (byte *)(void *)&tag;
    int realIndex = TAG_OFFSET + (index * sizeof(tag));
    unsigned int i;
    for (i = 0; i < sizeof(tag); i++)
        *p++ = EEPROM.read(realIndex++);
    return i;
}

int EEPROM_initialize(const musicBox_config &value)
{

    EEPROM_writeConfig(value);
    EEPROM_writeTag(0, FREE, value.folderCount - 1, 1);
    for (uint8_t i = 1; i < value.folderCount - 1; i++)
    {
        EEPROM_writeTag(i, FREE, i - 1, i + 1);
    }
    EEPROM_writeTag(value.folderCount - 1, FREE, value.folderCount - 2, 0);
}

int EEPROM_writeConfig(const musicBox_config &value)
{
    int ee = CONFIG_OFFSET;
    const byte *p = (const byte *)(const void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

int EEPROM_getConfig(musicBox_config &value)
{
    int ee = CONFIG_OFFSET;
    byte *p = (byte *)(void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}