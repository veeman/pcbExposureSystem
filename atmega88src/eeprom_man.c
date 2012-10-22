/**
 * Exposure System Project
 *
 * Copyright (c) 2009-2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 * All rights reserved.
 *
 */

#include "main.h"

uint8_t values[12] EEMEM = {30, 0, 60, 0, 120, 0, 180, 0, 240, 0, 5, 0xFF};

void eepload(void)
{
    uint16_t *ptr = 0;
    uint16_t value;

    do
    {
      value = eeprom_read_word((uint16_t*)ptr);
      ptr++;
    } while (((int8_t)(value >> 8) >= 0) && ((int)ptr < 512));

    time_cnt = value & 0xFF;
    if (time_cnt > 64) time_cnt = 0;

    if (time_cnt > 0)
    {
        uint8_t i;
        ptr--;
        for (i=0; i<time_cnt; ++i)
        {
            if (ptr==0)
              ptr = (uint16_t*)510;
            else
              ptr--;

            time_items[time_cnt-i-1] = eeprom_read_word(ptr);
        }
    }
}

void eesave()
{
    uint16_t *ptr = 0;
    uint16_t value;

    do
    {
      value = eeprom_read_word((uint16_t*)ptr);
      ptr++;
    } while (((int8_t)(value >> 8) >= 0) && ((int)ptr < 512));

    uint8_t i;
    ptr--;
    for (i=0; i<time_cnt; ++i)
    {
        eeprom_write_word(ptr, time_items[i]);
        ptr++;
        if ((int)ptr == 512) ptr = (uint16_t*)0;
    }

    eeprom_write_word(ptr, 0xFF00 | time_cnt);
}
