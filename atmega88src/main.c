/**
 * Exposure System Project
 *
 * Copyright (c) 2009-2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 * All rights reserved.
 *
 */
#include "main.h"

// ****************************************************************************
// constants
// ****************************************************************************
const char str_logo[]       PROGMEM = "C VitalijM 2010";
const char str_load[]       PROGMEM = "Loading ...";
const char str_exp_time[]   PROGMEM = "Belichtungszeit:";
const char str_new[]        PROGMEM = " Neue ";
const char str_idx[]        PROGMEM = " %d:";
const char str_time[]       PROGMEM = " %02d:%02d ";
const char str_new_time[]   PROGMEM = "Zeit:";
const char str_start[]      PROGMEM = " Start ";
const char str_save[]       PROGMEM = " Hinzuf\365gen ";
const char str_delete[]     PROGMEM = " L\357schen ";
const char str_lightning[]  PROGMEM = "Belichte";
const char str_abort[]      PROGMEM = " Abbrechen ";
const char str_aborted[]    PROGMEM = "Abgebrochen!";
const char str_succeed[]    PROGMEM = "Fertig!";
const char str_pressok[]    PROGMEM = "OK dr\365cken ^^";
const char str_eesave[]     PROGMEM = " Speichern ";
const char str_eesaving[]   PROGMEM = "Sichere Daten";

// ****************************************************************************
// global variables
// ****************************************************************************
uint8_t  time_cnt       = 0;
uint16_t time_items[64] = {}; //{30, 60, 90, 120, 180};
int8_t   item_pos       = 0;
int16_t  current_time   = 0;
volatile uint8_t  flag  = 0;


// ****************************************************************************
// help funtions
// ****************************************************************************
void printtime(uint8_t index, uint16_t time)
{
    uint8_t min = time / 60;
    uint8_t sec = time % 60;
    char str[16];
    str[0] = 0;
    if (index)
        strcat_P(str, str_idx);
    strcat_P(str, str_time);
    if (index)
        sprintf(str, str, index, min, sec);
    else
        sprintf(str, str, min, sec);
    lcd_string(str, 0);
    return;
}

uint16_t compare_func_up (const void * a, const void * b)
{
  return ( *(uint16_t*)a - *(uint16_t*)b );
}

uint16_t compare_func_down (const void * a, const void * b)
{
  return ( *(uint16_t*)b - *(uint16_t*)a );
}

// ****************************************************************************
// entry point
// ****************************************************************************
int main(void)
{
    /// init lcd display
    lcd_init();
    lcd_string((char*)str_logo, 1);
    set_cursor(0,2);
    lcd_string((char*)str_load, 1);

    /// init input buttons
    input_init();

    /// init relais and sled
    DDRD  = 0b00000011;
    PORTD = 0b00000010;

    /// load data from eeprm
    eepload();

    /// check selected item
    if (time_cnt >= 64)
         item_pos = 1;
    else
         item_pos = 0;


    _delay_ms(2000);
    //***********************************************
    main_menu:                                       /// Hauptmenu
    {
        lcd_clear();
        lcd_string((char*)str_exp_time, 1);
        current_time = 120;
    main_menu_2:
        set_cursor(0,2);
        lcd_string("\177", 0);
            if (item_pos == 0)
                lcd_string((char*)str_new, 1);
            else
            if (item_pos == -1)
                lcd_string((char*)str_eesave, 1);
            else
                printtime(item_pos, time_items[item_pos-1]);
        lcd_string("\176", 0);
        lcd_clear_line();

        _delay_ms(250);
        while(1)
        {
            if (BTN_OK)
            {
                if (item_pos == 0)
                    goto menu_new;
                else
                if (item_pos == -1)
                {
                    lcd_clear();
                    lcd_string((char*)str_eesaving, 1);
                    eesave();
                    lcd_clear();
                    lcd_string((char*)str_succeed, 1);
                    set_cursor(0, 2);
                    lcd_string((char*)str_pressok, 1);
                    _delay_ms(250);
                    while ((!BTN_BACK) && (!BTN_OK));
                    goto main_menu;
                }
                else
                {
                    current_time =  time_items[item_pos-1];
                    goto menu_select;
                }
            }
            if (BTN_RIGHT)
            {
                item_pos++;
                if (item_pos > time_cnt)
                {
                    item_pos = -1;
                    if (time_cnt >= 64)
                        item_pos++;
                }
                goto main_menu_2;
            }
            if (BTN_LEFT)
            {
                item_pos--;
                if (item_pos < -1)
                    item_pos = time_cnt;
                else
                if ((item_pos < 1) && (time_cnt >= 64))
                    item_pos = time_cnt;
                goto main_menu_2;
            }

        }
    }
    //***********************************************
    menu_new:                                        /// New Time Menu
    {
        int8_t   clock_ary[4];
        int8_t   pos;
        uint16_t time_edit;

        pos = current_time / 60;
        clock_ary[0] = pos / 10;
        clock_ary[1] = pos % 10;
        pos = current_time % 60;
        clock_ary[2] = pos / 10;
        clock_ary[3] = pos % 10;

        pos = 0;
        lcd_clear();
        lcd_string((char*)str_new+1, 1);
        lcd_string((char*)str_new_time, 1);
        lcd_enable_cursor;
    menu_new_2:
        time_edit = (clock_ary[0] * 10 + clock_ary[1]) * 60 + clock_ary[2] * 10 + clock_ary[3];
        set_cursor(0,2);
        lcd_string("\177", 0);
        printtime(0, time_edit);
        lcd_string("\176", 0);

        if (pos > 1)
            set_cursor(3 + pos, 2);
        else
            set_cursor(2 + pos, 2);

        _delay_ms(250);
        while(1)
        {
            if (BTN_BACK)
            {
                lcd_disable_cursor;
                goto main_menu;
            }
            if (BTN_RIGHT)
            {
                pos++;
                if (pos > 3)
                    pos = 0;
                goto menu_new_2;
            }
            if (BTN_LEFT)
            {
                pos--;
                if (pos < 0)
                    pos = 3;
                goto menu_new_2;
            }
            if (BTN_UP)
            {
                uint8_t X = pos == 2 ? 5 : 9;
                clock_ary[pos]++;
                if (clock_ary[pos] > X)
                   clock_ary[pos] = 0;
                goto menu_new_2;
            }
            if (BTN_DOWN)
            {
                clock_ary[pos]--;
                if (clock_ary[pos] < 0)
                   clock_ary[pos] = pos == 2 ? 5 : 9;
                goto menu_new_2;
            }
            if (BTN_OK)
            {
                current_time = (clock_ary[0] * 10 + clock_ary[1]) * 60 + clock_ary[2] * 10 + clock_ary[3];
                if (current_time >= 30)
                {
                    lcd_disable_cursor;
                    goto menu_select;
                }
            }
        }
    }
    //***********************************************
    menu_select:                                     /// Select Menu
    {
        int8_t pos = 0;
        lcd_clear();
        lcd_string((char*)str_new_time, 1);
        printtime(0, current_time);
    menu_select_2:
        set_cursor(0,2);
        lcd_string("\177", 0);
        if (!pos)
            lcd_string((char*)str_start, 1);
        else
        {
            if (item_pos)
                lcd_string((char*)str_delete, 1);
            else
                lcd_string((char*)str_save, 1);
        }
        lcd_string("\176", 0);
        lcd_clear_line();

        _delay_ms(250);
        while(1)
        {
            if (BTN_BACK)
            {
                if (item_pos)
                     goto main_menu;
                else
                    goto menu_new;
            }
            if (BTN_RIGHT)
            {
                pos++;
                if (pos > 1)
                    pos = 0;
                goto menu_select_2;
            }
            if (BTN_LEFT)
            {
                pos--;
                if (pos < 0)
                    pos = 1;
                goto menu_select_2;
            }
            if (BTN_OK)
            {
                if (pos == 0)
                    goto menu_light;
                else
                {
                    if  (item_pos)                  // delete
                    {
                        time_items[item_pos-1] = 0;
                        qsort(time_items, time_cnt, sizeof(uint16_t), (__compar_fn_t)compare_func_down);
                        time_cnt--;
                        item_pos--;
                        qsort(time_items, time_cnt, sizeof(uint16_t), (__compar_fn_t)compare_func_up);
                        goto main_menu;
                    }
                    else                            // save
                    {
                        time_cnt++;
                        time_items[time_cnt-1] = current_time;
                        qsort(time_items, time_cnt, sizeof(uint16_t), (__compar_fn_t)compare_func_up);
                        goto main_menu;
                    }
                }
            }
        }
    }

    //***********************************************
    menu_light:                                      /// Lighting Menu
    {
        int8_t pos = 0;
        lcd_clear();
        lcd_string((char*)str_lightning, 1);

        /// enable relais and start timer
        TCCR1A = 0x00;
        TCCR1C = 0x00;
        TCCR1B = (1 << CS12) | (1 << CS10);
        TCNT1  = 46004;
        TIMSK1 = (1 << TOIE1);

        PORTD |= 0b000000001;
        PORTD &= ~0b00000010;

        sei();

    menu_light_2:
        set_cursor(0,2);
        lcd_string("\177", 0);
        if (!pos)
            printtime(0, current_time);
        else
            lcd_string((char*)str_abort, 1);
        lcd_string("\176", 0);
        lcd_clear_line();

        _delay_ms(250);
        while(1)
        {
            if (BTN_RIGHT)
            {
                pos++;
                if (pos > 1)
                    pos = 0;
                goto menu_light_2;
            }
            if (BTN_LEFT)
            {
                pos--;
                if (pos < 0)
                    pos = 1;
                goto menu_light_2;
            }
            if ((BTN_OK) && (pos==1))
                goto menu_light_end;
            if (flag)
            {
                flag = 0;
                if (current_time <= 0)
                    goto menu_light_end;
                else
                    goto menu_light_2;
            }
        }
    }

    //***********************************************
    menu_light_end:                                  /// Lighting Menu End
    {
        cli();
        PORTD &= ~0b000000001;
        PORTD |= 0b00000010;

        lcd_clear();
        if (current_time)
            lcd_string((char*)str_aborted, 1);
        else
            lcd_string((char*)str_succeed, 1);
        set_cursor(0, 2);
        lcd_string((char*)str_pressok, 1);
        _delay_ms(250);
        while ((!BTN_BACK) && (!BTN_OK));
        goto main_menu;
    }

    return 0;
}


ISR(TIMER1_OVF_vect)
{
    TCNT1  = 46004;
    current_time--;
    flag = 1;
}
