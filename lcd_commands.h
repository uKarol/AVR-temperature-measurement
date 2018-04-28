/*
 * lcd_commands.h
 *
 * Created: 19.08.2017 22:21:18
 *  Author: Karol
 */ 


#ifndef LCD_COMMANDS_H_
#define LCD_COMMANDS_H_

#define CLS (1<<0)

#define CURSOR_HOME (1<<1)

#define ENTRY_MODE (1<<2)
#define ENTRY_RIGHT (1<<1)
#define ENTRY_LEFT 0

#define DISPLAY (1<<3)
#define DISP_ON (1<<2)
#define DISP_OFF 0
#define CURSOR_ON (1<<1)
#define CURSOR_OFF 0
#define BLINK_ON (1<<0)
#define BLINK_OFF 0

#define CURSOR (1<<4)
#define CURSOR_STATIC (1<<3)
#define CURSOR_MOVE 0
#define CURSOR_MOVE_RIGHT (1<<2)
#define CURSOR_MOVE_LEFT 0

#define FUNCTION_SET (1<<5)
#define INTERFACE_8BIT (1<<4)
#define INTERFACE_4BIT 0
#define TWO_VERSE (1<<3)
#define ONE_VERSE 0
#define CHAR_5x10 (1<<2)
#define CHAR_5x7 0

#define CURSOR_LOCATE 0x80

#define COMMON_FUNCTION_SET (FUNCTION_SET)|(INTERFACE_4BIT)|(TWO_VERSE)|(CHAR_5x7)
#define COMMON_CURSOR_SET (DISPLAY)|(CURSOR_OFF)
#define COMMON_LCD_SET (DISPLAY)|(DISP_ON)|(CURSOR_ON)|(BLINK_ON)
#define COMMON_ENTRY_SET (ENTRY_MODE)|(ENTRY_RIGHT)






#endif /* LCD_COMMANDS_H_ */