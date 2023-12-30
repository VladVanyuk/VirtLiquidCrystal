#include "LiquidCrystal.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

void LiquidCrystal::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
                         uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                         uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    _rs_pin = rs;
    _rw_pin = rw;
    _enable_pin = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;
    _data_pins[4] = d4;
    _data_pins[5] = d5;
    _data_pins[6] = d6;
    _data_pins[7] = d7;

    if (fourbitmode)
        _displayfunction = LCD_4BIT_MODE | LCD_1_LINE | LCD_5x8DOTS;
    else
        _displayfunction = LCD_8BIT_MODE | LCD_1_LINE | LCD_5x8DOTS;

    //  begin(16, 1);    // Shouldn't call begin from constructor
}

void LiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t charsize = LCD_5x8DOTS)
{

    pinMode(_rs_pin, OUTPUT);
    // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
    if (_rw_pin != 255)
    {
        pinMode(_rw_pin, OUTPUT);
    }
    pinMode(_enable_pin, OUTPUT);

    // Do these once, instead of every time a character is drawn for speed reasons.
    for (int i = 0; i < ((_displayfunction & LCD_8BIT_MODE) ? 8 : 4); ++i)
    {
        pinMode(_data_pins[i], OUTPUT);
    }

    setRowOffsets(cols, lines);

   
    // Now we pull both RS and R/W low to begin commands
    digitalWrite(_rs_pin, LOW);
    digitalWrite(_enable_pin, LOW);
    if (_rw_pin != 255)
    {
        digitalWrite(_rw_pin, LOW);
    }

    
    VirtLiquidCrystal::begin(cols, lines, charsize);

    
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal::send(uint8_t value, uint8_t mode)
{
    digitalWrite(_rs_pin, mode);

    // if there is a RW pin indicated, set it low to Write
    if (_rw_pin != 255)
    {
        digitalWrite(_rw_pin, LOW);
    }

    if (_displayfunction & LCD_8BIT_MODE)
    {
        write8bits(value);
    }
    else
    {
        write4bits(value >> 4);
        write4bits(value);
    }
}

void LiquidCrystal::pulseEnable(void)
{
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(1);
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(1); // enable pulse must be >450ns
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(100); // commands need > 37us to settle
}

void LiquidCrystal::write4bits(uint8_t value)
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

void LiquidCrystal::write8bits(uint8_t value)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}