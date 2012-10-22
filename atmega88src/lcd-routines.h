// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
void lcd_data(unsigned char temp1);
void lcd_string(char *data, uint8_t InFlash);
void lcd_command(unsigned char temp1);
void lcd_enable(void);
void lcd_init(void);
void lcd_home(void);
void lcd_clear(void);
void lcd_clear_line(void);
void lcd_home(void);
void set_cursor(uint8_t x, uint8_t y);

// LCD Befehle

#define CLEAR_DISPLAY 0x01
#define CURSOR_HOME   0x02

// Pinbelegung für das LCD, an verwendete Pins anpassen

#define LCD_PORT      PORTB
#define LCD_DDR       DDRB
#define LCD_RS        PB4
#define LCD_EN        PB5
// DB4 bis DB7 des LCD sind mit PD0 bis PD3 des AVR verbunden

#define lcd_enable_cursor  lcd_command(0b00001111)
#define lcd_disable_cursor lcd_command(0b00001100)
