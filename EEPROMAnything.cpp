#include "EEPROMAnything.h"

bool EEPROM_updateTag(uint8_t index, TagType tagType)
{
    switch (tagType)
    {
    case FREE:
    {
        // validate tag on index is USED
        MusicBox_tag tag;
        EEPROM_getTag(index, tag);
        switch (tag.tagType)
        {
        case FREE:
            // tag to update is already set to free
            return false;
            break;
        case USED:
            // todo
            // get first available previous free tag
            // set this tag's next to previous tag's next
            // set previous next tag to this tag
            // get next tag
            // set its prevoius tag to this tag

            return true;
            break;
        default:
            // tag is not initialized in eeprom
            return false;
            break;
        }
        break;
    }
    case USED:
    {
        // validate tag on index is FREE
        MusicBox_tag tag;
        EEPROM_getTag(index, tag);
        switch (tag.tagType)
        {
        case FREE:
            // get previous tag and set its next to this tag's next
            // write previous
            MusicBox_tag tagPrev;
            EEPROM_getTag(tag.prev, tagPrev);
            tagPrev.next = tag.next;
            EEPROM_writeTag(tag.prev, FREE, tagPrev.prev, tagPrev.next);

            // get next tag and set its prevoius tag to this tag's previous
            // write next
            MusicBox_tag tagNext;
            EEPROM_getTag(tag.next, tagNext);
            tagNext.prev = tag.prev;
            EEPROM_writeTag(tag.next, FREE, tagNext.prev, tagNext.next);

            // set this tag to USED
            // write tag
            tag.tagType = USED;
            EEPROM_writeTag(index, tag.tagType, tag.prev, tag.next);

            return true;
            break;
        case USED:
            // tag to update is already set to free
            return false;
            break;
        default:
            // tag is not initialized in eeprom
            return false;
            break;
        }
        break;
    }
    default:
        // unknown tag type
        return false;
        break;
    }
}

uint8_t EEPROM_getNextFree(uint8_t index)
{
    MusicBox_tag tag;
    EEPROM_getTag(index, tag);
    return tag.next;
}

uint8_t EEPROM_getPreviousFree(uint8_t index)
{
    MusicBox_tag tag;
    EEPROM_getTag(index, tag);
    return tag.prev;
}

int EEPROM_writeTag(uint8_t index, TagType tagType, uint8_t prev, uint8_t next)
{
    MusicBox_tag tag;
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

int EEPROM_getTag(uint8_t index, MusicBox_tag &tag)
{
    byte *p = (byte *)(void *)&tag;
    int realIndex = TAG_OFFSET + (index * sizeof(tag));
    unsigned int i;
    for (i = 0; i < sizeof(tag); i++)
        *p++ = EEPROM.read(realIndex++);
    return i;
}

void EEPROM_initialize(const MusicBox_config &value)
{
    // write config and set all tags in eeprom to free
    EEPROM_writeConfig(value);
    EEPROM_writeTag(0, FREE, value.folderCount - 1, 1);
    for (uint8_t i = 1; i < value.folderCount - 1; i++)
    {
        EEPROM_writeTag(i, FREE, i - 1, i + 1);
    }
    EEPROM_writeTag(value.folderCount - 1, FREE, value.folderCount - 2, 0);
}

int EEPROM_writeConfig(const MusicBox_config &value)
{
    int ee = CONFIG_OFFSET;
    const byte *p = (const byte *)(const void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

int EEPROM_getConfig(MusicBox_config &value)
{
    int ee = CONFIG_OFFSET;
    byte *p = (byte *)(void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}