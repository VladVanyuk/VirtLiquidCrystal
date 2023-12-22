/**
 * @file LCD.h
 * @brief Arduino LCD library for parallel and I2C LCDs.
 * This library provides an interface for controlling parallel LCD displays
 * that are compatible with the Hitachi HD44780 driver as well as I2C based LCDs.
 */

#ifndef _VirtLiquidCrystal_H_
#define _VirtLiquidCrystal_H_

#if (ARDUINO < 100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#include <inttypes.h>
#include <Print.h>

/*!
 @defined
 @abstract   Performs a bitwise shift.
 @discussion Defines _BV bit shift which is very dependent macro defined by
 Atmel.

    \note The bit shift is performed by the compiler which then inserts the
    result into the code. Thus, there is no run-time overhead when using
    _BV().
*/
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifdef __AVR__
#define FAST_MODE
#endif


// ---------------------------------------------------------------------------
/**
 * @defgroup LCD_Commands
 * @brief  All these definitions shouldn't be used unless you are writing a driver.
 * @note All these definitions are for driver implementation only and shouldn't be used by applications.
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// ---------------------------------------------------------------------------
/**
 * @defgroup flags for display entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// En = 0b00000100 # Enable bit
// Rw = 0b00000010 # Read/Write bit
// Rs = 0b00000001 # Register select bit

// ---------------------------------------------------------------------------
/**
 * @defgroup flags for display on/off and blink control
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00

#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// # flags for backlight control
// LCD_BACKLIGHT = 0x08
// LCD_NOBACKLIGHT = 0x00
#define LCD_BACKLIGHT_ON         0x08// 0x01
#define LCD_BACKLIGHT_OFF        0x00

#define BACKLIGHT_OFF 0
#define BACKLIGHT_ON 255
// ---------------------------------------------------------------------------
/**
 * @defgroup flags for display/cursor shift
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// ---------------------------------------------------------------------------
/**
 * @defgroup flags for function set
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// ---------------------------------------------------------------------------
//new

/**
 * @defgroup Define COMMAND and DATA LCD Rs (used by send method).
 */
#define COMMAND 0
#define DATA 1
#define FOUR_BITS 2

/*!
 @defined
 @abstract   Defines the duration of the home and clear commands
 @discussion This constant defines the time it takes for the home and clear
 commands in the LCD - Time in microseconds.
 */
#define HOME_CLEAR_EXEC 2000




/*!
 @typedef t_backlighPol
 @abstract   Define backlight control polarity
 @discussion Backlight control polarity. @see setBacklightPin.
 */
typedef enum
{
   POSITIVE,
   NEGATIVE
} t_backlighPol;

/**
 * @class VirtLiquidCrystal
 * @brief Base LCD library class
 * This class encapsulates common functionality for working with LCD displays. It provides methods for writing to the display,
 * controlling the cursor, clearing the screen, turning the display on/off, configuring the backlight, etc.
 * Specific LCD device drivers should inherit from this base class and implement the hardware specific functionality.
 */
class VirtLiquidCrystal : public Print
{
public:

   /*
    @method
    @abstract   LiquidCrystal abstract constructor.
    @discussion LiquidCrystal class abstract constructor needed to create
    the base abstract class.
    */
   //LCD();

   /*
    * @function
    * @abstract   LCD initialization.
    * @discussion Initializes the LCD to a given size (col, row). This methods
    * initializes the LCD, therefore, it MUST be called prior to using any other
    * method from this class.
    *
    * This method is abstract, a base implementation is available common to all LCD
    * drivers. Should it not be compatible with some other LCD driver, a derived
    * implementation should be done on the driver specif class.
    *
    * @param      cols[in] the number of columns that the display has
    * @param      rows[in] the number of rows that the display has
    * @param      charsize[in] character size, default==LCD_5x8DOTS
    */
   virtual void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

   
   void clear();

  
   void home();

   
   void noDisplay();

   
   void display();

   
   void noBlink();

   
   void blink();

   
   void noCursor();

   
   void cursor();

   
   void scrollDisplayLeft();

   
   void scrollDisplayRight();

   
   void leftToRight();

   
   void rightToLeft();


   
   void autoscroll();

   
   void noAutoscroll();

   // void setRowOffsets(uint8_t cols, uint8_t lines);
   // void setRowOffsets(int row1, int row2, int row3, int row4);

   // void setDelay(uint8_t cmdDelay, uint8_t charDelay);
   void setDelay(uint8_t cmdDelay);

   
   void createChar(uint8_t location, uint8_t charmap[]); //(uint8_t, uint8_t[]);

#ifdef __AVR__
   
   void createChar(uint8_t location, const char *charmap);
#endif // __AVR__

   
   void setCursor(uint8_t col, uint8_t row);

   
#if (ARDUINO < 100)
   
   virtual void write(uint8_t value); //? ==================================================
   using Print::write;
#else
   virtual size_t write(uint8_t value);
   using Print::write;
#endif



   
   void backlight(void);

   
   void noBacklight(void);

   
   void on(void);

   
   void off(void);

   //
   // virtual class methods
   // --------------------------------------------------------------------------
   /*!
    @name setBacklightPin
    @brief   Sets the pin to control the backlight.
    @note Sets the pin in the device to control the backlight.
    This method is device dependent and can be programmed on each subclass. An
    empty function call is provided that does nothing.

    @param      value: pin associated to backlight control.
    @param      pol: backlight polarity control (POSITIVE, NEGATIVE)
    */
   virtual void setBacklightPin(uint8_t value, t_backlighPol pol){};

   /*!
    @name
    @brief   Sets the pin to control the backlight.
    @note Sets the pin in the device to control the backlight. The behaviour
    of this method is very dependent on the device. Some controllers support
    dimming some don't. Please read the actual header file for each individual
    device. The setBacklightPin method has to be called before setting the backlight
    or the adequate backlight control constructor.
    @see setBacklightPin.

    NOTE: The prefered methods to control the backlight are "backlight" and
    "noBacklight".

    @param      0..255 - the value is very dependent on the LCD. However,
    BACKLIGHT_OFF will be interpreted as off and BACKLIGHT_ON will drive the
    backlight on.
    */
   virtual void setBacklight(uint8_t new_val);

   //compatibility API function aliases
   virtual void blink_on();						// alias for blink()
   virtual void blink_off();       					// alias for noBlink()
   virtual void cursor_on();      	 					// alias for cursor()
   virtual void cursor_off();      					// alias for noCursor()
   virtual void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
   virtual void printstr(const char[]);
   virtual uint8_t status();

   
   virtual void setContrast(uint8_t new_val);
   

   uint8_t init_bargraph(uint8_t graphtype);
   void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
   void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
	 


protected:
   // Internal LCD variables to control the LCD shared between all derived classes.
   uint8_t _displayfunction; // LCD_5x10DOTS or LCD_5x8DOTS, LCD_4BITMODE or LCD_8BITMODE, LCD_1LINE or LCD_2LINE
   uint8_t _displaycontrol;  // LCD base control command LCD on/off, blink, cursor all commands are "ored" to its contents.
   uint8_t _displaymode;     // Text entry mode to the LCD
   uint8_t _numlines;        // Number of lines of the LCD, initialized with begin()
   alias uint8_t _rows = _numlines; // Number of rows in the LCD
   uint8_t _cols;            // Number of columns in the LCD
   // uint8_t _currline;        // Current line of the LCD
   t_backlighPol _polarity;  // Backlight polarity
   uint8_t _backlightValue;
   // uint8_t _initialized;
//   uint8_t _row_offsets[4];

private:
   
   void command(uint8_t value);

   /*!
    @function
    @abstract   Send a particular value to the LCD.
    @discussion Sends a particular value to the LCD. This is a pure abstract
    method, therefore, it is implementation dependent of each derived class how
    to physically write to the LCD.

    Users should never call this method.

    @param      value[in] Value to send to the LCD.
    @result     mode LOW - write to the LCD CGRAM, HIGH - write a command to
    the LCD.
    */
#if (ARDUINO < 100)
   virtual void send(uint8_t value, uint8_t mode){};
#else
   virtual void send(uint8_t value, uint8_t mode) = 0;
#endif

   virtual void write4bits(uint8_t) = 0;
   virtual void write8bits(uint8_t) = 0;

   virtual void pulseEnable() = 0;
};

#endif //! _LCD_H_