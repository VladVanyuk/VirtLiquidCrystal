
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
uint8_t VirtLiquidCrystal::init(uint8_t cols, uint8_t lines, uint8_t charsize = LCD_5x8DOTS)
{
   if (lines > 1)
   {
      _displayfunction |= LCD_2LINE;
   }

   _cols = cols;
   _rows = lines;
   _charsize = charsize;
  // _polarity = POSITIVE;
  // _backlightValue = LCD_BACKLIGHT_ON;
   _initialized = true;
   return _initialized;
}


void VirtLiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t charsize = LCD_5x8DOTS)
{  
   
   // for some 1 line displays you can select a 10 pixel high font
   // ------------------------------------------------------------
   if ((_charsize != LCD_5x8DOTS) && (_rows == 1))
   {
      _displayfunction |= LCD_5x10DOTS;
   }


   // ---------------------------------------------------------------------------
   // delay (100); // 100ms delay
   waitMicroseconds(100000);

   // put the LCD into 4 bit or 8 bit mode
   //  -------------------------------------

   if (!(_displayfunction & LCD_8BITMODE))
   {
    
      send(0x03, FOUR_BITS);
      waitMicroseconds(4500); // wait min 4.1ms

      // second try
      send(0x03, FOUR_BITS);
      waitMicroseconds(150); // waitMicroseconds min 100us

      // third go!
      send(0x03, FOUR_BITS);
      waitMicroseconds(150); // waitMicroseconds min of 100us

      // finally, set to 4-bit interface
      send(0x02, FOUR_BITS);
      waitMicroseconds(150); // waitMicroseconds min of 100us
   }
   else
   {
      // Send function set command sequence
      command(LCD_FUNCTIONSET | _displayfunction);
      waitMicroseconds(4500); // waitMicroseconds more than 4.1ms

      // second try
      command(LCD_FUNCTIONSET | _displayfunction);
      waitMicroseconds(150);

      // third go
      command(LCD_FUNCTIONSET | _displayfunction);
      waitMicroseconds(150);
   }

   // finally, set # lines, font size, etc.
   command(LCD_FUNCTIONSET | _displayfunction);
   waitMicroseconds(60); // waitMicroseconds more

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
   waitMicroseconds(HOME_CLEAR_EXEC); // this command is time consuming
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
   waitMicroseconds(HOME_CLEAR_EXEC); // This command is time consuming
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
   if (row >= _rows)
   {
      row = _rows - 1; // rows start at 0
   }

   // 16x4 LCDs have special memory map layout
   // ----------------------------------------
   if (_cols == 16 && _rows == 4)
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
void VirtLiquidCrystal::autoscroll(void)
{
   _displaymode |= LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void VirtLiquidCrystal::noAutoscroll(void)
{
   _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// Write to CGRAM of new characters
void VirtLiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
   location &= 0x7; // we only have 8 locations 0-7

   command(LCD_SETCGRAMADDR | (location << 3));
   waitMicroseconds(30);

   for (uint8_t i = 0; i < 8; i++)
   {
      write(charmap[i]); // call the virtual write method
      waitMicroseconds(40);
   }
}

#ifdef __AVR__
void VirtLiquidCrystal::createChar(uint8_t location, const char *charmap)
{
   location &= 0x7; // we only have 8 memory locations 0-7

   command(LCD_SETCGRAMADDR | (location << 3));
   waitMicroseconds(30);

   for (uint8_t i = 0; i < 8; i++)
   {
      write(pgm_read_byte_near(charmap++));
      waitMicroseconds(40);
   }
}
#endif // __AVR__

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
   default:
      return;
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
   default:
      return;
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

void VirtLiquidCrystal::waitMicroseconds(uint8_t cmdDelay)
{
#ifdef RTOS
   task_wait(cmdDelay);
#else
   delayMicroseconds(cmdDelay);
#endif
}

