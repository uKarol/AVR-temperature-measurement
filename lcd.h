/*
 * lcd.h
 *
 * Created: 19.08.2017 21:39:45
 *  Author: Karol
 */ 


#ifndef LCD_H_
#define LCD_H_

void cursor_home();
void cursor_on();
void cursor_off();
void blink_on();
void blink_off();
void lcd_put_char(char ch);
void lcd_put_string(char *str);
void lcd_cls();
void lcd_init();
void lcd_locate(char x, char y);
void lcd_defchar(unsigned char nr, char *defchar);
void lcd_put_long( long number, unsigned char radix);
void lcd_put_int( int number, unsigned char radix);
void Slide_text(char *str);


#endif /* LCD_H_ */