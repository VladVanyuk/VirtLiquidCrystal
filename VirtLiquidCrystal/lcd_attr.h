
// Refactored Code
enum DisplayCommands
{
    CLEAR_DISPLAY = 0x01,      // Clears entire display and return home.
    RETURN_HOME = 0x02,        // Sets DDRAM 0 in address counter.
    ENTRY_MODE_SET = 0x04,     // Sets cursor move direction and display shift.
    CONTROL_SET = 0x08,        // Set display, cursor and blinking controls.
    SHIFT_SET = 0x10,          // Set cursor and shifts display.
    FUNCTION_SET = 0x20,       // Sets interface data length, line and font.
    SET_CGRAM_ADDR = 0x40,     // Sets CGRAM address.
    SET_CGRAM_MASK = 0x3f,     // Mask (6-bit).
    SET_DDRAM_ADDR = 0x80,     // Sets DDRAM address.
    SET_DDRAM_MASK = 0x7f,     // Mask (7-bit).
    BIAS_RESISTOR_SET = 0x04,  // Bias resistor select.
    BIAS_RESISTOR_MASK = 0x03, // Mask (2-bit).
    COM_SEG_SET = 0x40,        // COM SEG direction select.
    COM_SET_MASK = 0x0f,       // Mask (4 bit).
    SET_DDATA_LENGTH = 0x80,   // Set display data length.
    SET_DDATA_MASK = 0x7f      // Mask (7 bit, 0..79 => 1..80).
};

enum EntryModeSetAttributes
{
    DISPLAY_SHIFT = 0x01, // Shift the entire display not cursor.
    INCREMENT = 0x02,     // Increment (right) on write.
    DECREMENT = 0x00      // Decrement (left) on write.
};

enum ControlSetAttributes
{
    BLINK_ON = 0x01,  // The character indicated by cursor blinks.
    CURSOR_ON = 0x02, // The cursor is displayed.
    DISPLAY_ON = 0x04 // The display is on.
};

enum ShiftSetAttributes
{
    MOVE_LEFT = 0x00,  // Moves cursor and shifts display.
    MOVE_RIGHT = 0x04, // without changing DDRAM contents.
    CURSOR_MODE = 0x00,
    DISPLAY_MOVE = 0x08
};

enum FunctionSetAttributes
{
    DATA_LENGTH_4BITS = 0x00, // Sets the interface data length, 4-bit or.
    DATA_LENGTH_8BITS = 0x10, // 8-bit.
    NR_LINES_1 = 0x00,        // Sets the number of display lines, 1 or.
    NR_LINES_2 = 0x08,        // 2.
    FONT_5X8DOTS = 0x00,      // Sets the character font, 5X8 dots or.
    FONT_5X10DOTS = 0x04,     // 5X10 dots.
    BASIC_SET = 0x00,         // Sets basic instruction set.
    EXTENDED_SET = 0x04       // Extended instruction set.
};
