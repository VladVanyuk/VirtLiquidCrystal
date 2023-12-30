// ---------------------------------------------------------------------------
// Created by Francisco Malpartida on 20/08/11.
// Copyright (C) - 2018
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License v3.0
//    along with this program.
//    If not, see <https://www.gnu.org/licenses/gpl-3.0.en.html>.
//
// ---------------------------------------------------------------------------
//
// Thread Safe: No
// Extendable: Yes
//
// @file LCD.h
// This file implements a basic liquid crystal library that comes as standard
// in the Arduino SDK.
//
// @brief
// This is a basic implementation of the LiquidCrystal library of the
// Arduino SDK. This library is a refactored version of the one supplied
// in the Arduino SDK in such a way that it simplifies its extension
// to support other mechanism to communicate to LCDs such as I2C, Serial, SR,
// The original library has been reworked in such a way that this will be
// the base class implementing all generic methods to command an LCD based
// on the Hitachi HD44780 and compatible chipsets.
//
// This base class is a pure abstract class and needs to be extended. As reference,
// it has been extended to drive 4 and 8 bit mode control, LCDs and I2C extension
// backpacks such as the I2CLCDextraIO using the PCF8574* I2C IO Expander ASIC.
//
// The functionality provided by this class and its base class is identical
// to the original functionality of the Arduino LiquidCrystal library.
//
// @version API 1.1.0
//
//
// @author F. Malpartida - fmalpartida@gmail.com
// ---------------------------------------------------------------------------
#ifndef _LCD_H_
#define _LCD_H_

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

inline static void waitUsec(uint16_t uSec)
{
#ifndef FAST_MODE
   delayMicroseconds(uSec);
#endif // FAST_MODE
}

// LCD Commands
// ---------------------------------------------------------------------------
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
// ---------------------------------------------------------------------------
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off and cursor control
// ---------------------------------------------------------------------------
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
// ---------------------------------------------------------------------------
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
// ---------------------------------------------------------------------------
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Define COMMAND and DATA LCD Rs (used by send method).
// ---------------------------------------------------------------------------
#define COMMAND 0
#define LCD_DATA 1
#define FOUR_BITS 2

#define HOME_CLEAR_EXEC 2000

#define BACKLIGHT_OFF 0

#define BACKLIGHT_ON 255

typedef enum
{
   POSITIVE,
   NEGATIVE
} t_backlightPol;

class LCD : public Print
{
public:
   LCD();

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

   void moveCursorLeft();

   void moveCursorRight();

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

   virtual void setBacklightPin(uint8_t pin, t_backlightPol pol){};

   virtual void setBacklight(uint8_t value){};

#if (ARDUINO < 100)
   virtual void write(uint8_t value);
#else
   virtual size_t write(uint8_t value);
#endif

#if (ARDUINO < 100)
   using Print::write;
#else
   using Print::write;
#endif

protected:
   uint8_t _displayfunction; // LCD_5x10DOTS or LCD_5x8DOTS, LCD_4BITMODE or
                             // LCD_8BITMODE, LCD_1LINE or LCD_2LINE

   uint8_t _displaycontrol;  // LCD base control command LCD on/off, blink, cursor
                             // all commands are "ored" to its contents.

   uint8_t _displaymode;     // Text entry mode to the LCD
   uint8_t _numlines;        // Number of lines of the LCD, initialized with begin()
   uint8_t _cols;            // Number of columns in the LCD
   t_backlightPol _polarity; // Backlight polarity

private:
   void command(uint8_t value);

#if (ARDUINO < 100)
   virtual void send(uint8_t value, uint8_t mode){};
#else
   virtual void send(uint8_t value, uint8_t mode) = 0;
#endif
};

#endif
