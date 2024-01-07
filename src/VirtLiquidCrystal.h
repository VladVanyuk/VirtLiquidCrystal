/**
 * @file VirtLiquidCrystal.h
 * @brief Arduino LCD library for library provides an interface for controlling parallel LCD displays
 * that are compatible with the Hitachi HD44780 driver as well as I2C based LCDs.
 */

#ifndef _VirtLiquidCrystal_H_
#define _VirtLiquidCrystal_H_

#if (ARDUINO < 100)
// #include <WProgram.h>
#else
#include <Arduino.h>
#endif

#ifdef __AVR__
// #include <avr/pgmspace.h>
#endif

#include <inttypes.h>
#include <Print.h> 

#include "LCD_Defines.h"
#include "LCD_BaseTypes.h"
#include "utils.h"


#ifdef __AVR__
#define FAST_MODE
#endif



class VirtLiquidCrystal : public Print
{
public:

  VirtLiquidCrystal();

  uint8_t init(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  void begin();

  void clear();
  void home();
  void noDisplay();
  void display();

  void display(lcd_mode_t mode);

  void noBlink();
  void blink();

  void noCursor();
  void cursor();

  void scrollDisplayLeft(); 
  void scrollDisplayRight(); 

  void leftToRight();
  void rightToLeft();

  void moveCursorRight(); 
  void moveCursorLeft();

  void autoscroll();
  void noAutoscroll();


  void createChar(uint8_t location, uint8_t charmap[]);

#ifdef __AVR__
  void createChar(uint8_t location, const char *charmap);
#endif // __AVR__


  void setCursor(uint8_t col, uint8_t row);

  void backlight(void);
  void noBacklight(void);

  void on(void);
  void off(void);

  void waitMicroseconds(uint8_t cmdDelay);
  //& Virtual class methods --------------------------------------------------------------------------
  
  
#if (ARDUINO < 100)
  virtual void write(uint8_t value);
  virtual void setBacklightPin(uint8_t pin, t_backlightPol pol = POSITIVE){};
  virtual void setBacklight(uint8_t new_val){};
#else
  virtual size_t write(uint8_t value);
   virtual void setBacklightPin(uint8_t pin, t_backlightPol pol = POSITIVE) = 0;
   virtual void setBacklight(uint8_t new_val) = 0;
  
#endif
  void command(uint8_t value);

  using Print::write;

  //   //& Internal LCD variables to control the LCD shared between all derived classes. --------------------------------------------------------------------------

  uint8_t _displayfunction; // LCD_5x10DOTS or LCD_5x8DOTS, LCD_4BIT_MODE or LCD_8BIT_MODE, LCD_1_LINE or LCD_2_LINE
  uint8_t _displaycontrol;  // LCD base control command LCD on/off, blink, cursor all commands are "ored" to its contents.
  uint8_t _displaymode;     // Text entry mode to the LCD

  uint8_t _charsize;
  uint8_t _rows;
  uint8_t _cols;

  t_backlightPol _polarity;
  uint8_t _backlightValue;
  // uint8_t _backlightPin;

  uint8_t _En; //_enable_pin // 
  uint8_t _Rw; // _rw_pin // R/W pin
  uint8_t _Rs; // _rs_pin//  Register Select pin

protected:
  uint8_t _initialized;

  //& PRIVATE--------------------------------------------------------------------------

private:
#if (ARDUINO < 100)
  virtual void send(uint8_t value, uint8_t mode){};
   //virtual void pulseEnable(void){};
#else
  // virtual void send(uint8_t value, uint8_t mode) = 0;
  virtual void send(uint8_t value, uint8_t mode) = 0;
  // virtual void pulseEnable(void) = 0;
 // virtual void pulseEnable(void) = 0;
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
