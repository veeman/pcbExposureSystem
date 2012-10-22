/**
 * Exposure System Project
 *
 * Copyright (c) 2009-2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 * All rights reserved.
 *
 */

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

void input_init(void);

#define BUTTON_PORT     PINC
#define BUTTON_LEFT     PC2
#define BUTTON_RIGHT    PC4
#define BUTTON_UP       PC1
#define BUTTON_DOWN     PC5
#define BUTTON_OK       PC3
#define BUTTON_BACK     PC0

#define BTN_LEFT  !(BUTTON_PORT & (1 << BUTTON_LEFT))
#define BTN_RIGHT !(BUTTON_PORT & (1 << BUTTON_RIGHT))
#define BTN_UP    !(BUTTON_PORT & (1 << BUTTON_UP))
#define BTN_DOWN  !(BUTTON_PORT & (1 << BUTTON_DOWN))
#define BTN_OK    !(BUTTON_PORT & (1 << BUTTON_OK))
#define BTN_BACK  !(BUTTON_PORT & (1 << BUTTON_BACK))


#endif // INPUT_H_INCLUDED
