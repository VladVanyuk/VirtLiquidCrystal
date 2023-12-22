
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO < 100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// extern "C" void __cxa_pure_virtual() { while (1); }
#include "VirtLiquidCrystal.h"

// PUBLIC METHODS
// ---------------------------------------------------------------------------
// When the display powers up, it is configured as follows:
// 0. LCD starts in 8 bit mode
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
// can't assume that its in that state when a application starts (and the
// LiquidCrystal constructor is called).
// A call to begin() will reinitialize the LCD.
//
void VirtLiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
   if (lines > 1)
   {
      _displayfunction |= LCD_2LINE;
   }
   _numlines = lines;
   _cols = cols;

   // for some 1 line displays you can select a 10 pixel high font
   // ------------------------------------------------------------
   if ((dotsize != LCD_5x8DOTS) && (lines == 1))
   {
      _displayfunction |= LCD_5x10DOTS;
   }

   // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
   // according to datasheet, we need at least 40ms after power rises above 2.7V
   // before sending commands. Arduino can turn on way before 4.5V so we'll wait
   // 50
   // ---------------------------------------------------------------------------
   // delay (100); // 100ms delay
   setDelay(100000);

   // put the LCD into 4 bit or 8 bit mode
   //  -------------------------------------

   if (!(_displayfunction & LCD_8BITMODE))
   {
      // this is according to the hitachi HD44780 datasheet
      // figure 24, pg 46

      // we start in 8bit mode, try to set 4 bit mode
      // Special case of "Function Set"
      send(0x03, FOUR_BITS);
      setDelay(4500); // wait min 4.1ms

      // second try
      send(0x03, FOUR_BITS);
      setDelay(150); // wait min 100us

      // third go!
      send(0x03, FOUR_BITS);
      setDelay(150); // wait min of 100us

      // finally, set to 4-bit interface
      send(0x02, FOUR_BITS);
      setDelay(150); // wait min of 100us
   }
   else
   {
      // this is according to the hitachi HD44780 datasheet
      // page 45 figure 23

      // Send function set command sequence
      command(LCD_FUNCTIONSET | _displayfunction);
      setDelay(4500); // wait more than 4.1ms

      // second try
      command(LCD_FUNCTIONSET | _displayfunction);
      setDelay(150);

      // third go
      command(LCD_FUNCTIONSET | _displayfunction);
      setDelay(150);
   }

   // finally, set # lines, font size, etc.
   command(LCD_FUNCTIONSET | _displayfunction);
   setDelay(60); // wait more

   // turn the display on with no cursor or blinking default
   _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
   display();

   // clear the LCD
   clear();

   // Initialize to default text direction (for romance languages)
   _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
   // set the entry mode
   command(LCD_ENTRYMODESET | _displaymode);

   backlight();
}

// Common LCD Commands
// ---------------------------------------------------------------------------

/*!
    @function
    @abstract   Clears the LCD.
    @discussion Clears the LCD screen and positions the cursor in the upper-left
    corner.

    This operation is time consuming for the LCD.

    @param      none
    */
void VirtLiquidCrystal::clear()
{
   command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
   setDelay(HOME_CLEAR_EXEC); // this command is time consuming
}

 /*!
    @function
    @abstract   Sets the cursor to the upper-left corner.
    @discussion Positions the cursor in the upper-left of the LCD.
    That is, use that location in outputting subsequent text to the display.
    To also clear the display, use the clear() function instead.

    This operation is time consuming for the LCD.

    @param      none
    */
void VirtLiquidCrystal::home()
{
   command(LCD_RETURNHOME);   // set cursor position to zero
   setDelay(HOME_CLEAR_EXEC); // This command is time consuming
}

/*!
    @function
    @abstract   Position the LCD cursor.
    @discussion Sets the position of the LCD cursor. Set the location at which
    subsequent text written to the LCD will be displayed.

    @param      col[in] LCD column
    @param      row[in] LCD row - line.
    */
void VirtLiquidCrystal::setCursor(uint8_t col, uint8_t row)
{

   // const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets); // uint8_t _row_offsets[4];
   if (row >= _numlines)
   {
      row = _numlines - 1; // rows start at 0
   }

   // 16x4 LCDs have special memory map layout
   // ----------------------------------------
   if (_cols == 16 && _numlines == 4)
   {
      const byte row_offsetsLarge[] = {0x00, 0x40, 0x10, 0x50}; // For 16x4 LCDs
      command(LCD_SETDDRAMADDR | (col + row_offsetsLarge[row]));
   }
   else
   {
      const byte row_offsetsDef[] = {0x00, 0x40, 0x14, 0x54}; // For regular LCDs
      command(LCD_SETDDRAMADDR | (col + row_offsetsDef[row]));
   }
}

// Turn the display on/off
/*!
    @function
    @abstract   Turns off the LCD display.
    @discussion Turns off the LCD display, without losing the text currently
    being displayed on it.

    @param      none
    */
void VirtLiquidCrystal::noDisplay()
{
   _displaycontrol &= ~LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*!
    @function
    @abstract   Turns on the LCD display.
    @discussion Turns on the LCD display, after it's been turned off with
    noDisplay(). This will restore the text (and cursor location) that was on
    the display prior to calling noDisplay().

    @param      none
    */
void VirtLiquidCrystal::display()
{
   _displaycontrol |= LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
/*!
    @function
    @abstract   Hides the LCD cursor.

    @param      none
    */
void VirtLiquidCrystal::noCursor()
{
   _displaycontrol &= ~LCD_CURSORON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*!
    @function
    @abstract   Display the LCD cursor.
    @discussion Display the LCD cursor: an underscore (line) at the location
    where the next character will be written.

    @param      none
    */
void VirtLiquidCrystal::cursor()
{
   _displaycontrol |= LCD_CURSORON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns on/off the blinking cursor
/*!
    @function
    @abstract   Turns off the blinking of the LCD cursor.

    @param      none
    */
void VirtLiquidCrystal::noBlink()
{
   _displaycontrol &= ~LCD_BLINKON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*!
    @function
    @abstract   Display the cursor of the LCD.
    @discussion Display the blinking LCD cursor. If used in combination with
    the cursor() function, the result will depend on the particular display.

    @param      none
    */
void VirtLiquidCrystal::blink()
{
   _displaycontrol |= LCD_BLINKON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
/*!
    @function
    @abstract   Moves the cursor one space to the left.
    @discussion
    @param      none
    */
   /*!
    @function
    @abstract   Scrolls the contents of the display (text and cursor) one space
    to the left.

    @param      none
    */
void VirtLiquidCrystal::scrollDisplayLeft(void)
{
   command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

// This is for text that flows Left to Right
/*!
    @function
    @abstract   Set the direction for text written to the LCD to left-to-right.
    @discussion Set the direction for text written to the LCD to left-to-right.
    All subsequent characters written to the display will go from left to right,
    but does not affect previously-output text.

    This is the default configuration.

    @param      none
    */
void VirtLiquidCrystal::leftToRight(void)
{
   _displaymode |= LCD_ENTRYLEFT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
/*!
    @function
    @abstract   Set the direction for text written to the LCD to right-to-left.
    @discussion Set the direction for text written to the LCD to right-to-left.
    All subsequent characters written to the display will go from right to left,
    but does not affect previously-output text.

    left-to-right is the default configuration.

    @param      none
    */
void VirtLiquidCrystal::rightToLeft(void)
{
   _displaymode &= ~LCD_ENTRYLEFT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This method moves the cursor one space to the right
/*!
    @function
    @abstract   Moves the cursor one space to the right.

    @param      none
    */
   /*!
    @function
    @abstract   Scrolls the contents of the display (text and cursor) one space
    to the right.

    @param      none
    */
void VirtLiquidCrystal::scrollDisplayRight(void)
{
   command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
}

// This method moves the cursor one space to the left
void VirtLiquidCrystal::moveCursorLeft(void)
{
   command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
}

// This will 'right justify' text from the cursor
/*!
    @function
    @abstract   Turns on automatic scrolling of the LCD.
    @discussion Turns on automatic scrolling of the LCD. This causes each
    character output to the display to push previous characters over by one
    space. If the current text direction is left-to-right (the default),
    the display scrolls to the left; if the current direction is right-to-left,
    the display scrolls to the right.
    This has the effect of outputting each new character to the same location on
    the LCD.

    @param      none
    */
void VirtLiquidCrystal::autoscroll(void)
{
   _displaymode |= LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
/*!
    @function
    @abstract   Turns off automatic scrolling of the LCD.
    @discussion Turns off automatic scrolling of the LCD, this is the default
    configuration of the LCD.

    @param      none
    */
void VirtLiquidCrystal::noAutoscroll(void)
{
   _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// Write to CGRAM of new characters
/*!
    @function
    @abstract   Creates a custom character for use on the LCD.
    @discussion Create a custom character (glyph) for use on the LCD.
    Most chipsets only support up to eight characters of 5x8 pixels. Therefore,
    this methods has been limited to locations (numbered 0 to 7).

    The appearance of each custom character is specified by an array of eight
    bytes, one for each row. The five least significant bits of each byte
    determine the pixels in that row. To display a custom character on screen,
    write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.

    @param      location[in] LCD memory location of the character to create
    (0 to 7)
    @param      charmap[in] the bitmap array representing each row of the character.
    */
void VirtLiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
   location &= 0x7; // we only have 8 locations 0-7

   command(LCD_SETCGRAMADDR | (location << 3));
   setDelay(30);

   for (uint8_t i = 0; i < 8; i++)
   {
      write(charmap[i]); // call the virtual write method
      setDelay(40);
   }
}

#ifdef __AVR__
/*!
    @function
    @abstract   Creates a custom character for use on the LCD.
    @discussion Create a custom character (glyph) for use on t{he LCD.
    Most chipsets only support up to eight characters of 5x8 pixels. Therefore,
    this methods has been limited to locations (numbered 0 to 7).

    The appearance of each custom character is specified by an array of eight
    bytes, one for each row. The five least significant bits of each byte
    determine the pixels in that row. To display a custom character on screen,
    write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.

    This method take the character defined in program memory.

    @param      location[in] LCD memory location of the character to create
    (0 to 7)
    @param      charmap[in] the bitmap array representing each row of the character.
                Usage for flash defined characters:
                const char str_pstr[] PROGMEM = {0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0};
    */
void VirtLiquidCrystal::createChar(uint8_t location, const char *charmap)
{
   location &= 0x7; // we only have 8 memory locations 0-7

   command(LCD_SETCGRAMADDR | (location << 3));
   setDelay(30);

   for (uint8_t i = 0; i < 8; i++)
   {
      write(pgm_read_byte_near(charmap++));
      setDelay(40);
   }
}
#endif // __AVR__

// void VirtLiquidCrystal::setBacklight(uint8_t new_val){
// 	if (new_val) {
// 		backlight();		// turn backlight on
// 	} else {
// 		noBacklight();		// turn backlight off
// 	}
// }

//
// Switch on the backlight
/*!
    @function
    @abstract   Switch-on the LCD backlight.
    @discussion Switch-on the LCD backlight.
    The setBacklightPin has to be called before setting the backlight for
    this method to work. @see setBacklightPin.
    */
void VirtLiquidCrystal::backlight(void)
{
   switch (_polarity)
   {
   case POSITIVE:
      _backlightValue = LCD_BACKLIGHT_ON;
      break;

   case NEGATIVE:
      _backlightValue = ~LCD_BACKLIGHT_ON;
      break;
   }
}

//
// Switch off the backlight
void VirtLiquidCrystal::noBacklight(void)
{
   switch (_polarity)
   {
   case POSITIVE:
      _backlightValue = LCD_BACKLIGHT_OFF;
      break;

   case NEGATIVE:
      _backlightValue = ~LCD_BACKLIGHT_OFF;
      break;
   }
}

//
// Switch fully on the LCD (backlight and LCD)
void VirtLiquidCrystal::on(void)
{
   display();
   backlight();
}

//
// Switch fully off the LCD (backlight and LCD)
void VirtLiquidCrystal::off(void)
{
   noBacklight();
   noDisplay();
}

//& General LCD commands - generic methods used by the rest of the commands
//& ---------------------------------------------------------------------------

void VirtLiquidCrystal::command(uint8_t value)
{
   send(value, COMMAND);
}

#if (ARDUINO < 100)
void VirtLiquidCrystal::write(uint8_t value)
{
   send(value, DATA);
}
#else
size_t VirtLiquidCrystal::write(uint8_t value)
{
   send(value, DATA);
   return 1; // assume OK
}
#endif

void VirtLiquidCrystal::setDelay(uint8_t cmdDelay)
{
#ifdef RTOS
   task_wait(cmdDelay);
#else
   delayMicroseconds(cmdDelay);
#endif
}

