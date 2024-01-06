
enum LCD_Commands
{
    LCD_CLEAR_DISPLAY = 0x01,      // 0b00000001 // (1<<0) // Clears entire display and return home.
    LCD_RETURN_HOME = 0x02,        // 0b00000010 // (1<<1) // Sets DDRAM 0 in address counter.
    LCD_ENTRY_MODE_SET = 0x04,     // 0b00000100 // (1<<2) // Sets cursor move direction and display shift.
    LCD_DISPLAY_CONTROL = 0x08,        // 0b00001000 // (1<<3) // Set display, cursor and blinking controls.
    LCD_CURSOR_SHIFT = 0x10,          // 0b00010000 // (1<<4) // Set cursor and shifts display.
    LCD_FUNCTION_SET = 0x20,       // 0b00100000 // (1<<5) // Sets interface data length, line and font.
    LCD_SET_CGRAM_ADDR = 0x40,     // 0b01000000 // (1<<6) // Sets CGRAM address.
    SET_CGRAM_MASK = 0x3f,     // 0b00111111 // / // Mask (6-bit).
    LCD_SET_DDRAM_ADDR = 0x80,     // 0b10000000 // (1<<7) // Sets DDRAM address.
    SET_DDRAM_MASK = 0x7f,     // 0b01111111 // , // Mask (7-bit).
    BIAS_RESISTOR_SET = 0x04,  // 0b00000100 // (1<<2) // Bias resistor select.
    BIAS_RESISTOR_MASK = 0x03, // 0b00000011 // , // Mask (2-bit).
    COM_SEG_SET = 0x40,        // 0b01000000 // (1<<6) // COM SEG direction select.
    COM_SET_MASK = 0x0f,       // 0b00001111 // , // Mask (4 bit).
    SET_DDATA_LENGTH = 0x80,   // 0b10000000 // (1<<7) // Set display data length.
    SET_DDATA_MASK = 0x7f      // 0b01111111 // , // Mask (7 bit, 0..79 => 1..80).
};

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

typedef enum
{
  POSITIVE,
  NEGATIVE
} t_backlighPol;

#ifdef 0

// commands
#define LCD_NOOP 0x00
#define LCD_CURSORHOME 0x01
#define LCD_CURSORPOS 0x02
#define LCD_CURSORPOSXY 0x03
#define LCD_CURSOROFF 0x04
#define LCD_CURSORON 0x05
#define LCD_CURSORBLINK 0x06
#define LCD_BACKSPACE 0x08
#define LCD_TAB 0x09
#define LCD_CURSORDOWN 0x0A
#define LCD_CURSORUP 0x0B
#define LCD_CLEARDISPLAY 0x0C
#define LCD_LINEFEED 0x0D
#define LCD_CLEARCOLUMN 0x11
#define LCD_TABSET 0x12
#define LCD_BACKLIGHTON 0x13
#define LCD_BACKLIGHTOFF 0x14
#define LCD_CUSTOMCHAR 0x1B

#endif