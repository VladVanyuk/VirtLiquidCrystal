#include "LiquidCrystal_I2C.h"

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

inline size_t LiquidCrystal_I2C::write(uint8_t value) 
{
	send(value, Rs);
	return 1;
}

#else
#include "WProgram.h"

inline void LiquidCrystal_I2C::write(uint8_t value) 
{
	send(value, Rs);
}

#endif


LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS)
{
  init(lcd_addr, lcd_cols, lcd_rows, charsize);
  
}

uint8_t LiquidCrystal_I2C::init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);
{
  I2CIO::init(lcd_addr);
  VirtLiquidCrystal::init(lcd_cols, lcd_rows, charsize);
}

void LiquidCrystal_I2C::begin()
{
  
  if (I2CIO::begin() == 1)
  {
    I2CIO::portMode ( OUTPUT );
  }


  _displayfunction = LCD_4BIT_MODE | LCD_1_LINE | LCD_5x8DOTS;

 

  // for some 1 line displays you can select a 10 pixel high font
  if ((_charsize != 0) && (_rows == 1))
  {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay(50);

  // Now we pull both RS and R/W low to begin commands
  expanderWrite(_backlightValue); // reset expanderand turn backlight off (Bit 8 =1)
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
  command(LCD_FUNCTION_SET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for roman languages)
  _displaymode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;

  // set the entry mode
  command(LCD_ENTRY_MODE_SET | _displaymode);

  home();
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C::noBacklight(void)
{
  VirtLiquidCrystal::noBacklight();
  expanderWrite(0);
}

void LiquidCrystal_I2C::backlight(void)
{
  VirtLiquidCrystal::backlight();
  expanderWrite(0);
}
bool LiquidCrystal_I2C::getBacklight()
{
  return _backlightValue == LCD_BACKLIGHT_ON;
}

/************ low level data pushing commands **********/

// expanderWrite either command or data
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode)
{
  uint8_t highnib = value & 0xf0;
  // uint8_t lownib = (value << 4) & 0xf0;
  // write4bits((highnib) | mode);
  // write4bits((lownib) | mode);

   if ( mode == FOUR_BITS )
   {
      write4bits( (value & 0x0F), COMMAND );
   }
   else 
   {
      write4bits( (value >> 4), mode );
      write4bits( (value & 0x0F), mode);
   }
}

void LiquidCrystal_I2C::write4bits(uint8_t value)
{
  expanderWrite(value);
  pulseEnable(value);
}

uint8_t LiquidCrystal_I2C::expanderWrite(uint8_t _data)
{
  return I2CIO::write((int)(_data) | _backlightValue);
}

void LiquidCrystal_I2C::pulseEnable(uint8_t _data)
{
  I2CIO::write(_data | En); // En high
  delayMicroseconds(1);      // enable pulse must be >450ns

  expanderWrite(_data & ~En); // En low
  delayMicroseconds(50);      // commands need > 37us to settle
}

void LiquidCrystal_I2C::load_custom_character(uint8_t char_num, uint8_t *rows)
{
  createChar(char_num, rows);
}

void LiquidCrystal_I2C::setBacklightPin ( uint8_t value, t_backlightPol pol = POSITIVE )
{
   //_backlightPinMask = ( 1 << value );
   _polarity = pol;
  //  setBacklight(BACKLIGHT_OFF); //todo
}

void LiquidCrystal_I2C::setBacklight(uint8_t new_val)
{
  if (new_val)
  {
    backlight(); // turn backlight on
  }
  else
  {
    noBacklight(); // turn backlight off
  }
}

void LiquidCrystal_I2C::printstr(const char c[])
{
  // This function is not identical to the function used for "real" I2C displays
  // it's here so the user sketch doesn't have to be changed
  print(c);
}