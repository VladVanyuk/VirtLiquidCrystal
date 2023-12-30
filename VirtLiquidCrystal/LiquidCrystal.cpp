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

LiquidCrystal::LiquidCrystal(uint8_t cols, uint8_t lines, uint8_t charsize = LCD_5x8DOTS,
                             uint8_t bitmode = LCD_4BIT_MODE, uint8_t rs, uint8_t rw = UINT8_MAX, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                             uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0,
                             uint8_t backlighPin = 0, t_backlighPol pol = POSITIVE)
{
    init(cols, lines, charsize, bitmode, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7, backlighPin, pol);
}

void LiquidCrystal::init(uint8_t cols, uint8_t lines, uint8_t charsize = LCD_5x8DOTS,
                         uint8_t bitmode = LCD_4BIT_MODE, uint8_t rs, uint8_t rw = UINT8_MAX, uint8_t enable,
                         uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                         uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0,
                         uint8_t backlighPin = 0, t_backlighPol pol = POSITIVE)
{

    _Rs = rs;
    _Rw = rw;
    _En = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;
    _data_pins[4] = d4;
    _data_pins[5] = d5;
    _data_pins[6] = d6;
    _data_pins[7] = d7;

    _displayfunction = bitmode | LCD_1_LINE | charsize;

    if (backlighPin)
    {
        setBacklightPin(backlighPin, pol);
    }
    else
    {
        _backlightPin = 0;
        _polarity = pol;
    }

    VirtLiquidCrystal::init(cols, lines, charsize);
}

void LiquidCrystal::setBacklightPin(uint8_t pin, t_backlightPol pol = POSITIVE)
{
    pinMode(pin, OUTPUT); // Difine the backlight pin as output
    _backlightPin = pin;
    _polarity = pol;
    setBacklight(BACKLIGHT_OFF); // Set the backlight low by default
}

// ESP32 complains if not included
#if defined(ARDUINO_ARCH_ESP32)
void LiquidCrystal::analogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = UINT8_MAX)
{
    // calculate duty, 8191 from 2 ^ 13 - 1
    uint32_t duty = (8191 / valueMax) * min(value, valueMax);

    // write duty to LEDC
    ledcWrite(channel, duty);
}
#endif

void LiquidCrystal::setBacklight(uint8_t value)
{
    // Check if there is a pin assigned to the backlight
    // ---------------------------------------------------
    if (_backlightPin != LCD_NOBACKLIGHT)
    {

#if digitalPinHasPWM
        if (digitalPinHasPWM(_backlightPin))
#elif digitalPinToTimer
        // On older 1.x Arduino have to check using hack
        if (digitalPinToTimer(_backlightPin) != NOT_ON_TIMER)
#else
        if (false) // if neither of the above we assume no PWM
#endif
        {
            // Check for control polarity inversion
            // ---------------------------------------------------
            if (_polarity == POSITIVE)
            {

                analogWrite(_backlightPin, value);
            }
            else
            {
                analogWrite(_backlightPin, UINT8_MAX - value);
            }
        }
        // Not a PWM pin, set the backlight pin for POSI or NEG
        // polarity
        // --------------------------------------------------------
        else if (((value > 0) && (_polarity == POSITIVE)) ||
                 ((value == 0) && (_polarity == NEGATIVE)))
        {
            digitalWrite(_backlightPin, HIGH);
        }
        else
        {
            digitalWrite(_backlightPin, LOW);
        }
    }
}

void LiquidCrystal::begin()
{
    pinMode(_Rs, OUTPUT);
    // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
    if (_Rw != UINT8_MAX)
    {
        pinMode(_Rw, OUTPUT);
    }

    pinMode(_En, OUTPUT);

    // Do these once, instead of every time a character is drawn for speed reasons.
    for (int i = 0; i < ((_displayfunction & LCD_8BIT_MODE) ? 8 : 4); ++i)
    {
        pinMode(_data_pins[i], OUTPUT);
    }

    // setRowOffsets(cols, lines);

    // Now we pull both RS and R/W low to begin commands
    digitalWrite(_Rs, LOW);
    digitalWrite(_En, LOW);
    if (_Rw != UINT8_MAX)
    {
        digitalWrite(_Rw, LOW);
    }

    VirtLiquidCrystal::begin();
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal::send(uint8_t value, uint8_t mode)
{
    digitalWrite(_Rs, (mode == LCD_DATA));

    // if there is a RW pin indicated, set it low to Write
    if (_Rw != UINT8_MAX)
    {
        digitalWrite(_Rw, LOW);
    }

    write(value);
}

void LiquidCrystal::write(uint8_t value)
{
    if (_displayfunction & LCD_8BIT_MODE)
    {
        write8bits(value);
    }
    else
    {
        write4bits(value >> 4);
        write4bits(value);
    }

    waitMicroseconds(EXEC_TIME);
}

void LiquidCrystal::pulseEnable(void)
{
    // There is no need for the delays, since the digitalWrite operation
    // takes longer.

    // digitalWrite(_En, LOW);
    // waitMicroseconds(1);
    digitalWrite(_En, HIGH);
    waitMicroseconds(1); // enable pulse must be >450ns
    digitalWrite(_En, LOW);
    // waitMicroseconds(100); // commands need > 37us to settle
}

void LiquidCrystal::write4bits(uint8_t value)
{
    writeNbits(value, 4);
}

void LiquidCrystal::write8bits(uint8_t value)
{
    writeNbits(value, 8);
}

void LiquidCrystal::writeNbits(uint8_t value, uint8_t numBits)
{
    for (uint8_t i = 0; i < numBits; i++)
    {
        digitalWrite(_data_pins[i], (value >> i) & LCD_ENTRY_SHIFT_INCREMENT);
    }
    pulseEnable();
}
