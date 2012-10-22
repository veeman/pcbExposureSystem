/**
 * Exposure System Project
 *
 * Copyright (c) 2009-2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 * All rights reserved.
 *
 */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/iom88.h>

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include "lcd-routines.h"
#include "input.h"
#include "eeprom_man.h"

extern uint8_t  time_cnt;
extern uint16_t time_items[64];

#endif // MAIN_H_INCLUDED
