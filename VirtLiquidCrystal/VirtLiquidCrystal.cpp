
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
      _displayfunction |= LCD_2_LINE;
   }

   _cols = cols;
   _rows = lines;
   _charsize = charsize;

   _initialized = true;
   return _initialized;
}


void VirtLiquidCrystal::begin()
{  
   if (!_initialized)
   {
      return;
   }
   
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

   if (!(_displayfunction & LCD_8BIT_MODE))
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
      command(LCD_FUNCTION_SET | _displayfunction);
      waitMicroseconds(4500); // waitMicroseconds more than 4.1ms

      // second try
      command(LCD_FUNCTION_SET | _displayfunction);
      waitMicroseconds(150);

      // third go
      command(LCD_FUNCTION_SET | _displayfunction);
      waitMicroseconds(150);
   }

   // finally, set # lines, font size, etc.
   command(LCD_FUNCTION_SET | _displayfunction);
   waitMicroseconds(60); // waitMicroseconds more

   // turn the display on with no cursor or blinking default
   _displaycontrol = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
   display();

   clear();

   _displaymode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
   command(LCD_ENTRY_MODE_SET | _displaymode);

   backlight();
}


void VirtLiquidCrystal::clear()
{
   command(LCD_CLEAR_DISPLAY); // clear display, set cursor position to zero
   waitMicroseconds(HOME_CLEAR_EXEC); // this command is time consuming
}


void VirtLiquidCrystal::home()
{
   command(LCD_RETURN_HOME);   // set cursor position to zero
   waitMicroseconds(HOME_CLEAR_EXEC); // This command is time consuming
}

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
      command(LCD_SET_DDRAM_ADDR | (col + row_offsetsLarge[row]));
   }
   else
   {
      const byte row_offsetsDef[] = {0x00, 0x40, 0x14, 0x54}; // For regular LCDs
      command(LCD_SET_DDRAM_ADDR | (col + row_offsetsDef[row]));
   }
}



void LCDi2c::display(lcd_mode_t mode) 
{
	switch(mode) 
   {
		case DISPLAY_ON :
			display();
			break;
		case DISPLAY_OFF:
			noDisplay();
			break;
		case CURSOR_ON:
			cursor();
			break;
		case CURSOR_OFF:
			noCursor();
			break;
		case BLINK_ON:
			blink();
			break;
		case BLINK_OFF:
			noBlink();
			break;
		case SCROLL_LEFT:
			scrollDisplayLeft();
			break;
		case SCROLL_RIGHT:
			scrollDisplayRight();
			break;
		case LEFT_TO_RIGHT:
			leftToRight();
			break;
		case RIGHT_TO_LEFT:
			rightToLeft();
			break;
		case AUTOSCROLL_ON:
			autoscroll();
			break;
		case AUTOSCROLL_OFF:
			noAutoscroll();
			break;
		case BACKLIGHT_ON:
			backlight();
			write(0);
			break;
		case BACKLIGHT_OFF:
			noBacklight();
			write(0);
			break;
		}
	}


void VirtLiquidCrystal::noDisplay()
{
   _displaycontrol &= ~LCD_DISPLAY_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}
void VirtLiquidCrystal::display()
{
   _displaycontrol |= LCD_DISPLAY_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}

void VirtLiquidCrystal::noCursor()
{
   _displaycontrol &= ~LCD_CURSOR_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}

void VirtLiquidCrystal::cursor()
{
   _displaycontrol |= LCD_CURSOR_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}

void VirtLiquidCrystal::noBlink()
{
   _displaycontrol &= ~LCD_BLINK_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}

void VirtLiquidCrystal::blink()
{
   _displaycontrol |= LCD_BLINK_ON;
   command(LCD_DISPLAY_CONTROL | _displaycontrol);
}


void VirtLiquidCrystal::scrollDisplayLeft(void) //moveCursorLeft
{
   command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
}

void VirtLiquidCrystal::scrollDisplayRight(void)
{
   command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
}
void VirtLiquidCrystal::leftToRight(void)
{
   _displaymode |= LCD_ENTRY_LEFT;
   command(LCD_ENTRY_MODE_SET | _displaymode);
}
void VirtLiquidCrystal::rightToLeft(void)
{
   _displaymode &= ~LCD_ENTRY_LEFT;
   command(LCD_ENTRY_MODE_SET | _displaymode);
}

// This method moves the cursor one space to the right
void VirtLiquidCrystal::moveCursorRight(void)
{
   command(LCD_CURSOR_SHIFT | LCD_CURSOR_MOVE | LCD_MOVE_RIGHT);
}

// This method moves the cursor one space to the left
void VirtLiquidCrystal::moveCursorLeft(void)
{
   command(LCD_CURSOR_SHIFT | LCD_CURSOR_MOVE | LCD_MOVE_LEFT);
}

void VirtLiquidCrystal::autoscroll(void)
{
   _displaymode |= LCD_ENTRY_SHIFT_INCREMENT;
   command(LCD_ENTRY_MODE_SET | _displaymode);
}

void VirtLiquidCrystal::noAutoscroll(void)
{
   _displaymode &= ~LCD_ENTRY_SHIFT_INCREMENT;
   command(LCD_ENTRY_MODE_SET | _displaymode);
}

// Write to CGRAM of new characters
void VirtLiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
   location &= 0x7; // we only have 8 locations 0-7

   command(LCD_SET_CGRAM_ADDR | (location << 3));
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

   command(LCD_SET_CGRAM_ADDR | (location << 3));
   waitMicroseconds(30);

   for (uint8_t i = 0; i < 8; i++)
   {
      write(pgm_read_byte_near(charmap++));
      waitMicroseconds(40);
   }
}
#endif // __AVR__


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

   setBacklight(LCD_BACKLIGHT);
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

   setBacklight(LCD_NOBACKLIGHT);
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
   send(value, LCD_DATA);
}
#else
size_t VirtLiquidCrystal::write(uint8_t value)
{
   send(value, LCD_DATA);
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

