// YWROBOT
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>
#include "Print.h"
#include <Wire.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100 // Enable bit
#define Rw B00000010 // Read/Write bit
#define Rs B00000001 // Register select bit

// #define En 0b00000100  // Enable bit
// #define Rw 0b00000010  // Read/Write bit
// #define Rs 0b00000001  // Register select bit

class LiquidCrystal_I2C : public Print
{
public:
    LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
    {
        _Addr = lcd_Addr;
        _cols = lcd_cols;
        _rows = lcd_rows;
        _backlightval = LCD_NOBACKLIGHT;
    }

    void init()
    {
        Wire.begin();
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
        begin(_cols, _rows);
    }

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS)
    {
        if (rows > 1)
        {
            _displayfunction |= LCD_2LINE;
        }
        _numlines = rows;

        // for some 1 line displays you can select a 10 pixel high font
        if ((charsize != LCD_5x8DOTS) && (rows == 1))
        {
            _displayfunction |= LCD_5x10DOTS;
        }

        // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
        // according to datasheet, we need at least 40ms after power rises above 2.7V
        // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
        delay(50);

        // Now we pull both RS and R/W low to begin commands
        expanderWrite(_backlightval); // reset expanderand turn backlight off (Bit 8 =1)
        delay(1000);

        // put the LCD into 4 bit mode
        //  this is according to the hitachi HD44780 datasheet
        //  figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(0x03 << 4);
        delayMicroseconds(4500); // wait min 4.1ms

        // second try
        write4bits(0x03 << 4);
        delayMicroseconds(4500); // wait min 4.1ms

        // third go!
        write4bits(0x03 << 4);
        delayMicroseconds(150);

        // finally, set to 4-bit interface
        write4bits(0x02 << 4);

        // set # lines, font size, etc.
        command(LCD_FUNCTIONSET | _displayfunction);

        // turn the display on with no cursor or blinking default
        _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
        display();

        // clear it off
        clear();

        // Initialize to default text direction (for roman languages)
        _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

        // set the entry mode
        command(LCD_ENTRYMODESET | _displaymode);

        home();
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

    void shiftIncrement();

    void shiftDecrement();

    void noBacklight()
    {
        _backlightval = LCD_NOBACKLIGHT;
        expanderWrite(0);
    }

    void backlight()
    {
        _backlightval = LCD_BACKLIGHT;
        expanderWrite(0);
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

    void createChar(uint8_t location, uint8_t charmap[])
    {
        location &= 0x7; // we only have 8 locations 0-7
        command(LCD_SETCGRAMADDR | (location << 3));
        for (int i = 0; i < 8; i++)
        {
            write(charmap[i]);
            // write(pgm_read_byte_near(charmap++));
        }
    }

    void setCursor(uint8_t col, uint8_t row)
    {
        int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        if (row > _numlines)
        {
            row = _numlines - 1; // we count rows starting w/0
        }
        command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
    }

#if defined(ARDUINO) && ARDUINO >= 100
    virtual size_t write(uint8_t);
#else
    virtual void write(uint8_t);
#endif
    void command(uint8_t);

    ////compatibility API function aliases
    void blink_on();                                             // alias for blink()
    void blink_off();                                            // alias for noBlink()
    void cursor_on();                                            // alias for cursor()
    void cursor_off();                                           // alias for noCursor()
    void setBacklight(uint8_t new_val);                          // alias for backlight() and nobacklight()
    void load_custom_character(uint8_t char_num, uint8_t *rows); // alias for createChar()
    void printstr(const char[]);

    ////Unsupported API functions (not implemented in this library)
    uint8_t status();
    void setContrast(uint8_t new_val);
    uint8_t keypad();
    void setDelay(int, int);
    void on();
    void off();
    uint8_t init_bargraph(uint8_t graphtype);
    void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
    void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);

private:
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void expanderWrite(uint8_t);
    void pulseEnable(uint8_t);
    uint8_t _Addr;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _backlightval;
};

#endif