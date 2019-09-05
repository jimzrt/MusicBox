#include "EEPROMUtility.h"

bool EEPROM_updateTag(uint8_t index, TagType tagType, MusicBox_config &config)
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
        {

            uint8_t lowerBound = config.lowestFree;
            uint8_t upperBound = config.highestFree;
            uint8_t freeCount = config.freeCount;

            if (freeCount == 0)
            {
                EEPROM_writeTag(index, FREE, 255, 255);
                config.freeCount++;
                config.lowestFree = index;
                config.highestFree = index;
                EEPROM_writeConfig(config);
                return true;
            }

            uint8_t prevCandidate = index - 1;

            // if there is a lower free tag
            if (prevCandidate >= lowerBound)
            {
                MusicBox_tag prevTag;
                EEPROM_getTag(prevCandidate, prevTag);
                while (prevTag.tagType != FREE)
                {
                    prevCandidate--;
                    EEPROM_getTag(prevCandidate, prevTag);
                }

                // set previous tag's next to this tag
                // if previous tag's next is -1, then this tag's next is -1

                uint8_t oldNext = prevTag.next;
                EEPROM_writeTag(prevCandidate, FREE, prevTag.prev, index);
                if (prevTag.next != 255)
                {
                    // get previous tag's next
                    EEPROM_getTag(prevTag.next, prevTag);

                    // set next tag's previous to this tag
                    EEPROM_writeTag(oldNext, FREE, index, prevTag.next);
                }
                else
                {
                    // if previous tag's next is -1, then this tag is the new highest free
                    config.highestFree = index;
                }

                // write this tag
                EEPROM_writeTag(index, FREE, prevCandidate, oldNext);

                config.freeCount++;
                EEPROM_writeConfig(config);
                return true;
            }
            else
            {
                // no lower free tag available
                // find next free tag

                uint8_t nextCandidate = index + 1;
                MusicBox_tag nextTag;
                EEPROM_getTag(nextCandidate, nextTag);
                while (nextTag.tagType != FREE && nextCandidate < 255)
                {
                    nextCandidate++;
                    EEPROM_getTag(nextCandidate, nextTag);
                }

                if (nextCandidate == 255)
                {
                    Serial.println("ERROR ERROR > 255");
                    return false;
                }

                // set next tag's previous to this tag
                nextTag.prev = index;
                EEPROM_writeTag(nextCandidate, FREE, index, nextTag.next);

                //write this tag
                EEPROM_writeTag(index, FREE, 255, nextCandidate);

                config.freeCount++;
                config.lowestFree = index;
                EEPROM_writeConfig(config);
                return true;
            }

            break;
        }
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
            if (tag.prev != 255)
            {
                MusicBox_tag tagPrev;
                EEPROM_getTag(tag.prev, tagPrev);
                tagPrev.next = tag.next;
                EEPROM_writeTag(tag.prev, FREE, tagPrev.prev, tagPrev.next);
            }

            // get next tag and set its prevoius tag to this tag's previous
            // write next
            if (tag.next != 255)
            {

                MusicBox_tag tagNext;
                EEPROM_getTag(tag.next, tagNext);
                tagNext.prev = tag.prev;
                EEPROM_writeTag(tag.next, FREE, tagNext.prev, tagNext.next);
            }
            // set this tag to USED
            // write tag
            tag.tagType = USED;
            EEPROM_writeTag(index, tag.tagType, tag.prev, tag.next);

            config.freeCount--;
            if (index == config.lowestFree)
            {
                config.lowestFree = tag.next;
            }
            if (index == config.highestFree)
            {
                config.highestFree = tag.prev;
            }
            EEPROM_writeConfig(config);

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
    Serial.print("previuos free soing: ");
    Serial.println(tag.next);
    return tag.next != 255 ? tag.next : index;
}

uint8_t EEPROM_getPreviousFree(uint8_t index)
{
    MusicBox_tag tag;
    EEPROM_getTag(index, tag);
    Serial.print("previuos free soing: ");
    Serial.println(tag.prev);
    return tag.prev != 255 ? tag.prev : index;
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
    EEPROM_writeTag(1, FREE, 255, 2);
    for (uint8_t i = 2; i < value.folderCount; i++)
    {
        EEPROM_writeTag(i, FREE, i - 1, i + 1);
    }
    EEPROM_writeTag(value.folderCount, FREE, value.folderCount - 1, 255);
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