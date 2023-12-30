/**
 * @file VirtLiquidCrystal.h
 * @brief Arduino LCD library for library provides an interface for controlling parallel LCD displays
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

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifdef __AVR__
#define FAST_MODE
#endif

/** @defgroup LCD_Commands
 *  @brief LCD command definitions shouldn't be used unless you are writing a driver.
 *  @note All these definitions are for driver implementation only and shouldn't be used by applications.
 */
#define LCD_CLEARDISPLAY 0x01   // (1<<0)
#define LCD_RETURNHOME 0x02     // (1<<1)
#define LCD_ENTRYMODESET 0x04   // (1<<2)
#define LCD_DISPLAYCONTROL 0x08 // (1<<3)
#define LCD_CURSORSHIFT 0x10    // (1<<4)
#define LCD_FUNCTIONSET 0x20    // (1<<5)
#define LCD_SETCGRAMADDR 0x40   // (1<<6)
#define LCD_SETDDRAMADDR 0x80   // (1<<7)

/** @defgroup flags for display entry mode
 *  Flags for setting the text entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02

#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/** @defgroup flags for display on/off and blink control
 *  Flags for turning the display on/off and controlling the blink and cursor
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/** @defgroup flags for backlight control
 *  Flags for controlling the backlight of the display
 */
#define LCD_BACKLIGHT_ON 0x08
#define LCD_BACKLIGHT_OFF 0x00 // flags for backlight control

/** @defgroup flags for display/cursor shift
 *  Flags for shifting the display or cursor
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/** @defgroup flags for function set
 *  Flags for setting the function of the display
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/** @defgroup Define COMMAND and DATA LCD Rs (used by send method).
 *  Constants for distinguishing between commands and data sent to the LCD
 */
#define COMMAND 0
#define DATA 1
#define FOUR_BITS 2

/** @defined
 *  @abstract Defines the duration of the home and clear commands
 *  @discussion This constant defines the time it takes for the home and clear commands in the LCD - Time in microseconds.
 */
#define HOME_CLEAR_EXEC 2000

typedef enum
{
  POSITIVE,
  NEGATIVE
} t_backlighPol;

class VirtLiquidCrystal : public Print
{
public:
  uint8_t init(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  /** @brief Clear the display */
  void clear();

  /** @brief Move the cursor to the home position */
  void home();

  /** @brief Turn off the display */
  void noDisplay();

  /** @brief Turn on the display */
  void display();

  /** @brief Turn off the blink */
  void noBlink();

  /** @brief Turn on the blink */
  void blink();

  /** @brief Turn off the cursor */
  void noCursor();

  /** @brief Turn on the cursor */
  void cursor();

  /** @brief Scroll the display left */
  void scrollDisplayLeft();

  /** @brief Scroll the display right */
  void scrollDisplayRight();

  /** @brief Set the text direction to left-to-right */
  void leftToRight();

  /** @brief Set the text direction to right-to-left */
  void rightToLeft();

  /** @brief Turn on autoscrolling */
  void autoscroll();

  /** @brief Turn off autoscrolling */
  void noAutoscroll();

  /** @brief Create a custom character
   *
   *  @param location Location of the custom character
   *  @param charmap Character map for the custom character
   */
  void createChar(uint8_t location, uint8_t charmap[]);

#ifdef __AVR__
  void createChar(uint8_t location, const char *charmap);
#endif // __AVR__

  /** @brief Set the cursor position
   *
   *  @param col Column position of the cursor
   *  @param row Row position of the cursor
   */
  void setCursor(uint8_t col, uint8_t row);

  /** @brief Turn on the backlight */
  void backlight(void);

  /** @brief Turn off the backlight */
  void noBacklight(void);

  /** @brief Turn on the display */
  void on(void);

  /** @brief Turn off the display */
  void off(void);

  //& Virtual class methods --------------------------------------------------------------------------

  /** @brief Set the polarity of the backlight
   *
   *  @param value Value of the backlight polarity
   *  @param pol Polarity of the backlight
   */
  virtual void setBacklightPin(uint8_t value, t_backlighPol pol = POSITIVE) = 0;

  /** @brief Set the value of the backlight
   *
   *  @param new_val New value of the backlight
   */
  virtual void setBacklight(uint8_t new_val) = 0;

#if (ARDUINO < 100)
  virtual void write(uint8_t value);
#else
  virtual size_t write(uint8_t value);
#endif
  using Print::write;


//   //& Internal LCD variables to control the LCD shared between all derived classes. --------------------------------------------------------------------------

  uint8_t _displayfunction; // LCD_5x10DOTS or LCD_5x8DOTS, LCD_4BITMODE or LCD_8BITMODE, LCD_1LINE or LCD_2LINE
  uint8_t _displaycontrol;  // LCD base control command LCD on/off, blink, cursor all commands are "ored" to its contents.
  uint8_t _displaymode;     // Text entry mode to the LCD

  uint8_t _charsize;
  

  uint8_t _rows;
  uint8_t _cols;

  t_backlighPol _polarity;
  uint8_t _backlightValue;

  protected:
    uint8_t _initialized;

  //& PRIVATE--------------------------------------------------------------------------

private:
  /** @brief Send a command to the LCD
   *
   *  @param value Value of the command to send
   */
  void command(uint8_t value);

#if (ARDUINO < 100)
  virtual void send(uint8_t value, uint8_t mode){};
  virtual void write4bits(uint8_t){};
  virtual void write8bits(uint8_t){};
  virtual void pulseEnable(void){};
#else
  virtual void send(uint8_t value, uint8_t mode) = 0;
  virtual void write4bits(uint8_t) = 0;
  virtual void write8bits(uint8_t) = 0;
  virtual void pulseEnable(void) = 0;
#endif
};

#endif // _VirtLiquidCrystal_H_

#if 0
//& compatibility API function aliases --------------------------------------------------------------------------

virtual void blink_on(); // alias for blink()
virtual void blink_off(); // alias for noBlink()
virtual void cursor_on(); // alias for cursor()
virtual void cursor_off(); // alias for noCursor()
virtual void load_custom_character(uint8_t char_num, uint8_t *rows); // alias for createChar()
virtual void printstr(const char[]);
virtual uint8_t status();
virtual uint8_t init_bargraph(uint8_t graphtype);
virtual void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
virtual void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
#endif
