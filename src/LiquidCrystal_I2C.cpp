#include "I2C_IO.h"
#include "LiquidCrystal_I2C.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_addr = LCD_DEFAULT_ADDR, uint8_t lcd_cols = LCD_DEFAULT_COLS, uint8_t lcd_rows = LCD_DEFAULT_ROWS)
{
  Serial.println("constr start");
  init(lcd_addr, lcd_cols, lcd_rows);

  Serial.println("constr end");
}

uint8_t LiquidCrystal_I2C::init(uint8_t lcd_addr = LCD_DEFAULT_ADDR, uint8_t lcd_cols = LCD_DEFAULT_COLS, uint8_t lcd_rows = LCD_DEFAULT_ROWS,
                      uint8_t charsize = LCD_5x8DOTS, uint8_t En = LCD_EN, uint8_t Rw = LCD_RW, uint8_t Rs = LCD_RS,
                      uint8_t d4 = LCD_D4, uint8_t d5 = LCD_D5, uint8_t d6 = LCD_D6, uint8_t d7 = LCD_D7,
                      uint8_t backlighPin = 0, backlightPol_t pol = POSITIVE)
{
  Serial.println("ini start");
  I2C_IO::init(lcd_addr);
  _displayfunction = LCD_4BIT_MODE | LCD_1_LINE | charsize; //LCD_5x8DOTS
  VirtLiquidCrystal::init(lcd_cols, lcd_rows, charsize);
  Serial.println("ini end");

  config(En, Rw, Rs, d4, d5, d6, d7, backlighPin, pol);
}

void LiquidCrystal_I2C::config(uint8_t En, uint8_t Rw, uint8_t Rs,
            uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
            uint8_t backlighPin = 0, backlightPol_t pol = POSITIVE)
{
  Serial.println("conf start");
  _En = (1 << En);
  _Rw = (1 << Rw);
  _Rs = (1 << Rs);

  // Initialise pin mapping
  _data_pins[0] = (1 << d4);
  _data_pins[1] = (1 << d5);
  _data_pins[2] = (1 << d6);
  _data_pins[3] = (1 << d7);

  if (backlighPin)
  {
    setBacklightPin(backlighPin, pol);
  }
  else
  {
    _backlightPinMask = 0;
    _backlightStsMask = LCD_NOBACKLIGHT;
    _polarity = pol;
  }
  Serial.println("conf end");
}

void LiquidCrystal_I2C::begin()
{

  Serial.println("begin");
  if (!I2C_IO::begin())
  {
    Serial.println("begin err");
    return;
  }
  VirtLiquidCrystal::begin();
  // home();

  Serial.println("begin end");
}

void LiquidCrystal_I2C::setBacklightPin(uint8_t pin, backlightPol_t pol = POSITIVE)
{

  _backlightPinMask = (1 << pin);
  _polarity = pol;
  setBacklight(0); // todo
}

void LiquidCrystal_I2C::setBacklight(uint8_t value)
{
  // Check if backlight is available
  // ----------------------------------------------------
  if (_backlightPinMask != 0x0)
  {
    // Check for polarity to configure mask accordingly
    // ----------------------------------------------------------
    if (((_polarity == POSITIVE) && (value > 0)) ||
        ((_polarity == NEGATIVE) && (value == 0)))
    {
      _backlightStsMask = _backlightPinMask & LCD_BACKLIGHT;
    }
    else
    {
      _backlightStsMask = _backlightPinMask & LCD_NOBACKLIGHT;
    }
    I2C_IO::writeExpander(_backlightStsMask);
  }
}


uint8_t LiquidCrystal_I2C::getBacklight()
{
  return _backlightStsMask;
}

/************ low level data pushing commands **********/

// expanderWrite either command or data
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode)
{

  if (mode == FOUR_BITS)
  {
    write4bits((value & 0x0F), COMMAND);
  }
  else
  {
    write4bits((value >> 4), mode);
    write4bits((value & 0x0F), mode);
  }
}


void LiquidCrystal_I2C::write4bits(uint8_t value, uint8_t mode)
{
  uint8_t pinMapValue = 0;

  // Map the value to LCD pin mapping
  // --------------------------------
  for (uint8_t i = 0; i < 4; i++)
  {
    if ((value & 0x1) == 1)
    {
      pinMapValue |= _data_pins[i];
    }
    value = (value >> 1);
  }

  // Is it a command or data
  // -----------------------
  if (mode == LCD_DATA)
  {
    mode = _Rs;
  }

  pinMapValue |= mode | _backlightStsMask;
  pulseEnable(pinMapValue);
}

void LiquidCrystal_I2C::pulseEnable(uint8_t data)
{
  I2C_IO::writeExpander(data | _En); // En high
   delayMicroseconds(1);     // enable pulse must be >450ns

  I2C_IO::writeExpander(data & ~_En); // En low
  delayMicroseconds(50);     // commands need > 37us to settle
}

void LiquidCrystal_I2C::printstr(const char c[])
{
  // This function is not identical to the function used for "real" I2C displays
  // it's here so the user sketch doesn't have to be changed
  print(c);
}