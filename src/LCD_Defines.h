#ifndef LCD_Defines_h
#define LCD_Defines_h

/** @defgroup LCD_Commands
 *  @brief LCD command definitions shouldn't be used unless you are writing a driver.
 *  @note All these definitions are for driver implementation only and shouldn't be used by applications.
 */
// #define LCD_CLEAR_DISPLAY 0x01   //& (1<<0)
// #define LCD_RETURN_HOME 0x02     //& (1<<1)
// #define LCD_ENTRY_MODE_SET 0x04  //& (1<<2)
// #define LCD_DISPLAY_CONTROL 0x08 //& (1<<3)
// #define LCD_CURSOR_SHIFT 0x10    //& (1<<4)
// #define LCD_FUNCTION_SET 0x20    //& (1<<5)
// #define LCD_SET_CGRAM_ADDR 0x40  //& (1<<6)
// #define LCD_SET_DDRAM_ADDR 0x80  //& (1<<7)

/** @defgroup flags for display entry mode
 *  Flags for setting the text entry mode
 */
#define LCD_ENTRY_RIGHT 0x00
#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01 
#define LCD_ENTRY_SHIFT_DECREMENT 0x00 

/** @defgroup flags for display on/off and blink control
 *  Flags for turning the display on/off and controlling the blink and cursor
 */
#define LCD_DISPLAY_ON 0x04   //& 
#define LCD_DISPLAY_OFF 0x00  
#define LCD_CURSOR_ON 0x02 //& 
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01 //& 
#define LCD_BLINK_OFF 0x00

/** @defgroup flags for display/cursor shift
 *  Flags for shifting the display or cursor
 */
#define LCD_DISPLAY_MOVE 0x08  //& 
#define LCD_CURSOR_MOVE 0x00   //& 
#define LCD_MOVE_RIGHT 0x04  //& 
#define LCD_MOVE_LEFT 0x00  //& 

/** @defgroup flags for function set
 *  Flags for setting the function of the display
 */
#define LCD_8BIT_MODE 0x10  //& 
#define LCD_4BIT_MODE 0x00  //& 
#define LCD_2_LINE 0x08     //& 
#define LCD_1_LINE 0x00     //& 
#define LCD_5x10DOTS 0x04   //& 
#define LCD_5x8DOTS 0x00    //& 

/** @defgroup flags for backlight control
 *  Flags for controlling the backlight of the display
 */
#define LCD_BACKLIGHT_ON 0x08
#define LCD_BACKLIGHT_OFF 0x00 
// flags for backlight control
#define LCD_NOBACKLIGHT 0x00
#define LCD_BACKLIGHT 0xFF //

/** @defgroup Define COMMAND and LCD_DATA LCD Rs (used by send method).
 *  Constants for distinguishing between commands and data sent to the LCD
 */
#define COMMAND 0
#define LCD_DATA 1
#define FOUR_BITS 2

#define HOME_CLEAR_EXEC 2000


#endif //! LCD_Defines_h