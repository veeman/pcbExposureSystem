/**
 * Exposure System Project
 *
 * Copyright (c) 2009-2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 * All rights reserved.
 *
 */

#include "main.h"

void input_init(void)
{
    // enable input pins
    DDRC  &= ~0b00011111;
    PORTC |=  0b00111111;
}
