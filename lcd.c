/*
 * lcd.c
 *
 * Created: 19.08.2017 21:39:25
 *  Author: Karol
 */ 

/*
	*biblioteka do wyœwietlacza alfanumerycznego 2x16 zgodnego ze standardem HD44780
	*tryb 4 bitowy
	*tylko wyœwietlanie danych, bez odczytu
	*Aby ograniczyæ liczbê pinów wyœwietlacz jest podpiêty poprzez rejestr przesówny, dane s¹ wysy³ane za pomoc¹ SPI
*/
#include "lcd_pins.h"
#include "lcd_commands.h"
#include "lcd.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define LCD_X  16// Iloœæ kolumn
#define LCD_Y 2 //Iloœæ wierszy

#define VERSE1 0 //adres pierwszego wiersza
#define VERSE2 0x40 //adres drugiego wiersza

enum bitset {SET, CLEAR};
	
char lcd_x_position;
char lcd_y_position;	
	

// zadaniem funkcji jest przes³anie bitu przez SPI do rejestru przesuwnego

void SendSpi(unsigned char byte, enum bitset bs){
	
	static unsigned char currentbyte=0;
	
	if(bs == SET) currentbyte|= (1<<byte);
	else currentbyte&= ~(1<<byte);
	
	SPDR = currentbyte;
	while(!(SPSR & (1<<SPIF)));
	
	PORTB|= (1<<CS);
	PORTB&= ~(1<<CS);
}

// inicjalizacja modu³u SPI

void InitSpi(){
	
	DDRB|= (1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}

//wysy³anie po³ówki bajtu (wybra³em tryb pracy 4 bitowy)

void lcd_send_nibble(unsigned char nibble){
	
	if(nibble&(1<<0)) SendSpi(D4_PIN, SET);
	else SendSpi(D4_PIN, CLEAR);
	
	if(nibble&(1<<1)) SendSpi(D5_PIN, SET);
	else SendSpi(D5_PIN, CLEAR);
	
	if(nibble&(1<<2))  SendSpi(D6_PIN, SET);
	else SendSpi(D6_PIN, CLEAR);
	
	if(nibble&(1<<3))  SendSpi(D7_PIN, SET);
	else SendSpi(D7_PIN, CLEAR);
}

//funkcja wysy³aj¹ca bajt

void lcd_send_byte(unsigned char byte){
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(byte>>4);
	SendSpi(E_PIN, CLEAR);
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(byte);
	SendSpi(E_PIN, CLEAR);
	
	_delay_us(120);
}

//funkcja wysy³aj¹ca polecenie do sterownika wyœwietlacza

void lcd_send_command(unsigned char command){
	
	SendSpi(RS_PIN, CLEAR);
	lcd_send_byte(command);
}

//funkcja wysy³aj¹ca dane do wyœwietlenia

void lcd_send_data(unsigned char data){
	SendSpi(RS_PIN, SET);
	lcd_send_byte(data);
}

//inicjalizacja wyœwietlacza

void lcd_init(){
	
	InitSpi();
	
	SendSpi(RS_PIN, SET);
	SendSpi(E_PIN, SET);
	
	_delay_ms(15);
	
	SendSpi(E_PIN, CLEAR);
	SendSpi(RS_PIN, CLEAR);
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(0x03); // 8 bit
	SendSpi(E_PIN, CLEAR);
	_delay_ms(4.1);
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(0x03); // 8 bit
	SendSpi(E_PIN, CLEAR);
	_delay_us(100);
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(0x03); // 8 bit
	SendSpi(E_PIN, CLEAR);
	_delay_us(100);
	
	SendSpi(E_PIN, SET);
	lcd_send_nibble(0x02); // 4 bit
	SendSpi(E_PIN, CLEAR);
	_delay_us(100);
	
	lcd_send_command(COMMON_FUNCTION_SET);
	lcd_send_command(COMMON_CURSOR_SET);
	lcd_send_command(COMMON_LCD_SET);
	lcd_send_command(COMMON_ENTRY_SET);
	lcd_cls();

}

//czyszczenie ekranu

void lcd_cls(){
	lcd_x_position = 0;
	lcd_y_position = 0;
	lcd_send_command(CLS);
	_delay_ms(4.9);
}

//powrót kursora do pozycji 0,0

void cursor_home(){
	lcd_send_command(CURSOR_HOME);
	_delay_ms(4.9);
}

// w³¹czenie kursora

void cursor_on(){
	lcd_send_command(DISPLAY|DISP_ON|CURSOR_ON);
}

// wy³¹czenie kursora

void cursor_off(){
	lcd_send_command(DISPLAY|DISP_ON|CURSOR_OFF);
}

//w³¹czenie migania kursora

void blink_on(){
	lcd_send_command(DISPLAY|DISP_ON|CURSOR_ON|BLINK_ON);
}

//wy³¹czenie migania kursora

void blink_off(){
	lcd_send_command(DISPLAY|DISP_ON|CURSOR_ON|BLINK_OFF);
}




// wyœwietlanie liczby w okreœlonym systemie 

void lcd_put_int( int number, unsigned char radix){
	char temp[5]; // 5-maksymalna iloœæ cyfr dla int
	itoa(number, temp, radix);
	lcd_put_string(temp);
}

void lcd_put_long( long number, unsigned char radix){
	char temp[10]; // 5-maksymalna iloœæ cyfr dla int
	ltoa(number, temp, radix);
	lcd_put_string(temp);
}

//zmiana pozycji kursora

void lcd_locate(char x, char y){
	
	lcd_x_position = x;
	lcd_y_position = y;
	
	if(lcd_x_position>=LCD_X || lcd_x_position<0 || lcd_y_position>=LCD_Y || lcd_y_position<0) return;
	
	switch(y){
		case 0: 
			y = VERSE1;
			break;
		case 1:
			y = VERSE2;
			break;
		default:
		break;
	}
	lcd_send_command(CURSOR_LOCATE + y + x);
}

// wys³anie pojedynczego znaku

void lcd_put_char(char ch){
	if(lcd_x_position<LCD_X && lcd_x_position>=0 && lcd_y_position<LCD_Y && lcd_y_position>=0){
		lcd_locate(lcd_x_position, lcd_y_position);
		lcd_send_data((ch>=0x80 && ch <= 0x87) ? (ch & 0x07) : ch);
	}
	lcd_x_position++;
	
}

void lcd_put_string(char *str){
	char temp_char;
	unsigned char i=0;
	while( str[i] ){
		temp_char = str[i];
		lcd_put_char(temp_char);
		i++;
	}
}


void lcd_defchar(unsigned char nr, char *defchar){
	char temp_char; 
	unsigned char loop_ctr;
	
	lcd_send_command( 64+ ((nr&0x07)*8) );
	
	for(loop_ctr = 0; loop_ctr < 8; loop_ctr++){
		temp_char = *(defchar++);
		
		lcd_send_data(temp_char);
	}
}

void Slide_text(char *str){
	
	char loop_ctr;
	unsigned char lng = strlen(str);
	
	for(loop_ctr = 0; loop_ctr <= lng; loop_ctr++){
		lcd_locate(0-loop_ctr, 0);
		lcd_put_string(str);
		lcd_locate(lng-loop_ctr,0);
		lcd_put_string("            ");
		_delay_ms(300);
	}
}