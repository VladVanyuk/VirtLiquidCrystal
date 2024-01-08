#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Print.h>
#include <Arduino.h>

#include "LCD_Defs.h"

class LiquidCrystal : public Print
{
public:
    LiquidCrystal(uint8_t rs, uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)

    {
        init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
    }

    LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)

    {
        init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
    }

    LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
    {
        init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    }

    LiquidCrystal(uint8_t rs, uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
    {
        init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    }

    void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
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
            _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
        else
            _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

        begin(16, 1);
    }

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS)
    {
        if (lines > 1)
        {
            _displayfunction |= LCD_2LINE;
        }
        _numlines = lines;

        setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

        // for some 1 line displays you can select a 10 pixel high font
        if ((dotsize != LCD_5x8DOTS) && (lines == 1))
        {
            _displayfunction |= LCD_5x10DOTS;
        }

        pinMode(_rs_pin, OUTPUT);
        // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
        if (_rw_pin != 255)
        {
            pinMode(_rw_pin, OUTPUT);
        }
        pinMode(_enable_pin, OUTPUT);

        // Do these once, instead of every time a character is drawn for speed reasons.
        for (int i = 0; i < ((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i)
        {
            pinMode(_data_pins[i], OUTPUT);
        }

        // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
        // according to datasheet, we need at least 40 ms after power rises above 2.7 V
        // before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
        delayMicroseconds(50000);
        // Now we pull both RS and R/W low to begin commands
        digitalWrite(_rs_pin, LOW);
        digitalWrite(_enable_pin, LOW);
        if (_rw_pin != 255)
        {
            digitalWrite(_rw_pin, LOW);
        }

        // put the LCD into 4 bit or 8 bit mode
        if (!(_displayfunction & LCD_8BITMODE))
        {
            // this is according to the Hitachi HD44780 datasheet
            // figure 24, pg 46

            // we start in 8bit mode, try to set 4 bit mode
            write4bits(0x03);
            delayMicroseconds(4500); // wait min 4.1ms

            // second try
            write4bits(0x03);
            delayMicroseconds(4500); // wait min 4.1ms

            // third go!
            write4bits(0x03);
            delayMicroseconds(150);

            // finally, set to 4-bit interface
            write4bits(0x02);
        }
        else
        {
            // this is according to the Hitachi HD44780 datasheet
            // page 45 figure 23

            // Send function set command sequence
            command(LCD_FUNCTIONSET | _displayfunction);
            delayMicroseconds(4500); // wait more than 4.1 ms

            // second try
            command(LCD_FUNCTIONSET | _displayfunction);
            delayMicroseconds(150);

            // third go
            command(LCD_FUNCTIONSET | _displayfunction);
        }

        // finally, set # lines, font size, etc.
        command(LCD_FUNCTIONSET | _displayfunction);

        // turn the display on with no cursor or blinking default
        _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
        display();

        // clear it off
        clear();

        // Initialize to default text direction (for romance languages)
        _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
        // set the entry mode
        command(LCD_ENTRYMODESET | _displaymode);
    }

    void clear()
    {
        command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
        delayMicroseconds(2000);   // this command takes a long time!
    }

    void home()
    {
        command(LCD_RETURNHOME); // set cursor position to zero
        delayMicroseconds(2000); // this command takes a long time!
    }

    void noDisplay()
    {
        _displaycontrol &= ~LCD_DISPLAYON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void display()
    {
        _displaycontrol |= LCD_DISPLAYON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void noBlink()
    {
        _displaycontrol &= ~LCD_BLINKON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void blink()
    {
        _displaycontrol |= LCD_BLINKON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void noCursor()
    {
        _displaycontrol &= ~LCD_CURSORON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void cursor()
    {
        _displaycontrol |= LCD_CURSORON;
        command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

    void scrollDisplayLeft()
    {
        command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    }

    void scrollDisplayRight()
    {
        command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    }

    void leftToRight()
    {
        _displaymode |= LCD_ENTRYLEFT;
        command(LCD_ENTRYMODESET | _displaymode);
    }

    void rightToLeft()
    {
        _displaymode &= ~LCD_ENTRYLEFT;
        command(LCD_ENTRYMODESET | _displaymode);
    }

    void autoscroll()
    {
        _displaymode |= LCD_ENTRYSHIFTINCREMENT;
        command(LCD_ENTRYMODESET | _displaymode);
    }

    void noAutoscroll()
    {
        _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
        command(LCD_ENTRYMODESET | _displaymode);
    }

    void setRowOffsets(int row0, int row1, int row2, int row3)
    {
        _row_offsets[0] = row0;
        _row_offsets[1] = row1;
        _row_offsets[2] = row2;
        _row_offsets[3] = row3;
    }

    void createChar(uint8_t location, uint8_t charmap[])
    {
        location &= 0x7; // we only have 8 locations 0-7
        command(LCD_SETCGRAMADDR | (location << 3));
        for (int i = 0; i < 8; i++)
        {
            write(charmap[i]);
        }
    }

    void setCursor(uint8_t col, uint8_t row)
    {
        const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
        if (row >= max_lines)
        {
            row = max_lines - 1; // we count rows starting w/ 0
        }
        if (row >= _numlines)
        {
            row = _numlines - 1; // we count rows starting w/ 0
        }

        command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
    }

    virtual size_t write(uint8_t);
    // inline size_t LiquidCrystal::write(uint8_t value) {
    //   send(value, HIGH);
    //   return 1; // assume success
    // }

    void command(uint8_t);
    // inline void LiquidCrystal::command(uint8_t value) {
    //   send(value, LOW);
    // }

    using Print::write;

    // inline void blink_on() { blink(); }
	// inline void blink_off() { noBlink(); }
	// inline void cursor_on() { cursor(); }
	//inline void cursor_off() { noCursor(); }

private:
    void send(uint8_t value, uint8_t mode)
    {
        digitalWrite(_rs_pin, mode);

        // if there is a RW pin indicated, set it low to Write
        if (_rw_pin != 255)
        {
            digitalWrite(_rw_pin, LOW);
        }

        if (_displayfunction & LCD_8BITMODE)
        {
            write8bits(value);
        }
        else
        {
            write4bits(value >> 4);
            write4bits(value);
        }
    }

    void write4bits(uint8_t value)
    {
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(_data_pins[i], (value >> i) & 0x01);
        }

        pulseEnable();
    }

    void write8bits(uint8_t value)
    {
        for (int i = 0; i < 8; i++)
        {
            digitalWrite(_data_pins[i], (value >> i) & 0x01);
        }

        pulseEnable();
    }

    void pulseEnable()
    {
        digitalWrite(_enable_pin, LOW);
        delayMicroseconds(1);
        digitalWrite(_enable_pin, HIGH);
        delayMicroseconds(1); // enable pulse must be >450 ns
        digitalWrite(_enable_pin, LOW);
        delayMicroseconds(100); // commands need >37 us to settle
    }

    uint8_t _rs_pin;     // LOW: command. HIGH: character.
    uint8_t _rw_pin;     // LOW: write to LCD. HIGH: read from LCD.
    uint8_t _enable_pin; // activated by a HIGH pulse.
    uint8_t _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];
};

#endif
