#ifndef LCD_BASE_TYPES_H
#define LCD_BASE_TYPES_H

#include <Arduino.h>

typedef enum
{
  POSITIVE,
  NEGATIVE
} backlightPol_t;

typedef enum
{
  DISPLAY_ON,
  DISPLAY_OFF,
  CURSOR_ON,
  CURSOR_OFF,
  BLINK_ON,
  BLINK_OFF,
  SCROLL_LEFT,
  SCROLL_RIGHT,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
  AUTOSCROLL_ON,
  AUTOSCROLL_OFF,
  BACKLIGHT_ON,
  BACKLIGHT_OFF,
} lcd_mode_t;

enum LCD_Commands
{
  LCD_CLEAR_DISPLAY = 0x01,   // 0b00000001 // (1<<0) // Clears entire display and return home.
  LCD_RETURN_HOME = 0x02,     // 0b00000010 // (1<<1) // Sets DDRAM 0 in address counter.
  LCD_ENTRY_MODE_SET = 0x04,  // 0b00000100 // (1<<2) // Sets cursor move direction and display shift.
  LCD_DISPLAY_CONTROL = 0x08, // 0b00001000 // (1<<3) // Set display, cursor and blinking controls.
  LCD_CURSOR_SHIFT = 0x10,    // 0b00010000 // (1<<4) // Set cursor and shifts display.
  LCD_FUNCTION_SET = 0x20,    // 0b00100000 // (1<<5) // Sets interface data length, line and font.
  LCD_SET_CGRAM_ADDR = 0x40,  // 0b01000000 // (1<<6) // Sets CGRAM address.
  SET_CGRAM_MASK = 0x3f,      // 0b00111111 // / // Mask (6-bit).
  LCD_SET_DDRAM_ADDR = 0x80,  // 0b10000000 // (1<<7) // Sets DDRAM address.
  SET_DDRAM_MASK = 0x7f,      // 0b01111111 // , // Mask (7-bit).
  BIAS_RESISTOR_SET = 0x04,   // 0b00000100 // (1<<2) // Bias resistor select.
  BIAS_RESISTOR_MASK = 0x03,  // 0b00000011 // , // Mask (2-bit).
  COM_SEG_SET = 0x40,         // 0b 0100 0000 // (1<<6) // COM SEG direction select.
  COM_SET_MASK = 0x0f,        // 0b 0000 1111 // , // Mask (4 bit).
  SET_DDATA_LENGTH = 0x80,    // 0b10000000 // (1<<7) // Set display data length.
  SET_DDATA_MASK = 0x7f       // 0b01111111 // , // Mask (7 bit, 0..79 => 1..80).
};


// Backlight Control ,  Command Byte Code 
	enum LCDBackLight_e : uint8_t{
		LCDBackLightOnMask = 0x0F, /**< XXXX-1111 , Turn on Back light */
		LCDBackLightOffMask = 0x07 /**< XXXX-0111, Turn off Back light */
	}; 
	
	/*! Entry mode control  set command,  Command Byte Code */
	enum LCDEntryMode_e : uint8_t{
		LCDEntryModeOne = 0x04, /**< Display Shift :OFF Decrement Address Counter */
		LCDEntryModeTwo = 0x05, /**< Display Shift :ON  Decrement Address Counter */
		LCDEntryModeThree = 0x06, /**< Display Shift :OFF Increment Address Counter, default */
		LCDEntryModeFour = 0x07 /**<  Display Shift :ON Increment Address Counter */
	}; 

	/*! Cursor mode, Command Byte Code */
	enum LCDCursorType_e : uint8_t {
		LCDCursorTypeOff= 0x0C, /**< Make cursor invisible */
		LCDCursorTypeBlink = 0x0D, /**< Turn on blinking-block cursor */
		LCDCursorTypeOn = 0x0E,  /**< Turn on visible  underline cursor */
		LCDCursorTypeOnBlink  = 0x0F /**<Turn on blinking-block cursor + visible underline cursor */
	}; 
	
	/*! Direction mode for scroll and move */
	enum LCDDirectionType_e : uint8_t {
		LCDMoveRight= 1, /**< move or scroll right */
		LCDMoveLeft = 2   /**< move or scroll left */
	}; 

	/*! Line Row Number */
	enum LCDLineNumber_e : uint8_t{
		LCDLineNumberOne = 1,   /**< row 1 */
		LCDLineNumberTwo = 2,   /**< row 2 */
		LCDLineNumberThree = 3, /**< row 3 */
		LCDLineNumberFour = 4  /**<  row 4 */
	}; 


  /*!  priv DDRAM address's used to set cursor position  Note Private */
	enum LCDAddress_e : uint8_t {
		LCDLineAddressOne =  0x80,  /**< Line 1 */
		LCDLineAddressTwo =  0xC0, /**< Line 2 */
		LCDLineAddress3Col20 = 0x94, /**< Line 3 20x04 line 3 */
		LCDLineAddress4Col20 = 0xD4, /**< Line 4 20x04 line 4 */
		LCDLineAddress3Col16  = 0x90, /**< Line 3 16x04  untested, no part */
		LCDLineAddress4Col16  = 0xD0 /**< Line 4 16x04 untested, no part */
	}; 

	/*!  Command Bytes General  Note Private */
	enum LCDCmdBytesGeneral_e : uint8_t {
		LCDModeFourBit = 0x28, /**< Function set (4-bit interface, 2 lines, 5*7 Pixels) */
		LCDHomePosition  = 0x02, /**< Home (move cursor to top/left character position) */
		LCDDisplayOn = 0x0C,  /**< Restore the display (with cursor hidden) */
		LCDDisplayOff = 0x08, /**< Blank the display (without clearing) */
		LCDClearScreen = 0x01 /**< clear screen */
	};


#if 0


/** @defgroup flags for display entry mode
 *  Flags for setting the text entry mode
 */
enum EntryModeSetAttributes
{
    DISPLAY_SHIFT = 0x01, //*LCD_ENTRY_SHIFT_INCREMENT*/ LCD_ENTRY_SHIFT_DECREMENT // Shift the entire display not cursor.
    INCREMENT = 0x02, //*LCD_ENTRY_LEFT*/     // //Increment (right) on write.
    DECREMENT = 0x00  //*LCD_ENTRY_RIGHT*/     // Decrement (left) on write.
};

enum ControlSetAttributes
{
    LCD_BLINK_ON = 0x01,  // The character indicated by cursor blinks.
    LCD_CURSOR_ON = 0x02, // The cursor is displayed.
    LCD_DISPLAY_ON = 0x04 // The display is on. //~ LCD_DISPLAY_OFF 0x00
};

/** @defgroup flags for display/cursor shift
 *  Flags for shifting the display or cursor
 */
enum ShiftSetAttributes
{
    LCD_MOVE_LEFT = 0x00,  // Moves cursor and shifts display.
    LCD_MOVE_RIGHT = 0x04, // without changing DDRAM contents.
    LCD_CURSOR_MOVE = 0x00,
    LCD_DISPLAY_MOVE = 0x08
};

/** @defgroup flags for function set
 *  Flags for setting the function of the display
 */
enum FunctionSetAttributes
{
    LCD_4BIT_MODE = 0x00, // Sets the interface data length, 4-bit or.
    LCD_8BIT_MODE = 0x10, // 8-bit.
    LCD_1_LINE = 0x00,        // Sets the number of display lines, 1 or.
    LCD_2_LINE = 0x08,        // 2.
    LCD_5x8DOTS = 0x00,      // Sets the character font, 5X8 dots or.
    LCD_5x10DOTS = 0x04,     // 5X10 dots.
    BASIC_SET = 0x00,         // Sets basic instruction set.
    EXTENDED_SET = 0x04       // Extended instruction set.
};

#endif

#endif //! LCD_BASE_TYPES_H
