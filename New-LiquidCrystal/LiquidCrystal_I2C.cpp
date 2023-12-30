
#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <inttypes.h>
#include "I2C_IO.h"
#include "LiquidCrystal_I2C.h"

// CONSTANT  definitions
// ---------------------------------------------------------------------------

// flags for backlight control
#define LCD_NOBACKLIGHT 0x00
#define LCD_BACKLIGHT   0xFF



#define LCD_EN 6  // Enable bit
#define LCD_RW 5  // Read/Write bit
#define LCD_RS 4  // Register select bit
#define LCD_D4 0
#define LCD_D5 1
#define LCD_D6 2
#define LCD_D7 3


// CONSTRUCTORS
// ---------------------------------------------------------------------------

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En = EN, uint8_t Rw = RW,
                                     uint8_t Rs = RS, uint8_t d4 = D4, uint8_t d5 = D5,
                                     uint8_t d6 = D6, uint8_t d7 = D7, uint8_t backlighPin = 0, 
                                     t_backlightPol pol = POSITIVE )
{
   config(lcd_Addr, En, Rw, Rs, d4, d5, d6, d7, backlighPin, pol);
}


void LiquidCrystal_I2C::config (uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                                uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                                uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE)
{
   _Addr = lcd_Addr;

   _En = ( 1 << En );
   _Rw = ( 1 << Rw );
   _Rs = ( 1 << Rs );
   
   // Initialise pin mapping
   _data_pins[0] = ( 1 << d4 );
   _data_pins[1] = ( 1 << d5 );
   _data_pins[2] = ( 1 << d6 );
   _data_pins[3] = ( 1 << d7 );   

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
}



int LiquidCrystal_I2C::init()
{
   int status = 0;
   
   // initialize the backpack IO expander
   // and display functions.
   // ------------------------------------------------------------------------
   if ( _i2cio.begin ( _Addr ) == 1 )
   {
      _i2cio.portMode ( OUTPUT );  // Set the entire IO extender to OUTPUT
      _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
      status = 1;
      _i2cio.write(LOW);  // Set the entire port to LOW
   }
   return ( status );
}



void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize = LCD_5x8DOTS) 
{
   
   init();     // Initialise the I2C expander interface
   LCD::begin ( cols, lines, dotsize );   
}


void LiquidCrystal_I2C::setBacklightPin ( uint8_t pin, t_backlightPol pol = POSITIVE )
{
   _backlightPinMask = ( 1 << pin );
   _polarity = pol;
   setBacklight(BACKLIGHT_OFF);
}


void LiquidCrystal_I2C::setBacklight( uint8_t value ) 
{
   // Check if backlight is available
   // ----------------------------------------------------
   if ( _backlightPinMask != 0x0 )
   {
      // Check for polarity to configure mask accordingly
      // ----------------------------------------------------------
      if  (((_polarity == POSITIVE) && (value > 0)) || 
           ((_polarity == NEGATIVE ) && ( value == 0 )))
      {
         _backlightStsMask = _backlightPinMask & LCD_BACKLIGHT;
      }
      else 
      {
         _backlightStsMask = _backlightPinMask & LCD_NOBACKLIGHT;
      }
      _i2cio.write( _backlightStsMask );
   }
}




// low level data pushing commands
//----------------------------------------------------------------------------

//
// send - write either command or data
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) 
{
   // No need to use the delay routines since the time taken to write takes
   // longer that what is needed both for toggling and enable pin an to execute
   // the command.
   
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


void LiquidCrystal_I2C::write4bits ( uint8_t value, uint8_t mode ) 
{
   uint8_t pinMapValue = 0;
   
   // Map the value to LCD pin mapping
   // --------------------------------
   for ( uint8_t i = 0; i < 4; i++ )
   {
      if ( ( value & 0x1 ) == 1 )
      {
         pinMapValue |= _data_pins[i];
      }
      value = ( value >> 1 );
   }
   
   // Is it a command or data
   // -----------------------
   if ( mode == LCD_DATA )
   {
      mode = _Rs;
   }
   
   pinMapValue |= mode | _backlightStsMask;
   pulseEnable ( pinMapValue );
}

//
// pulseEnable
void LiquidCrystal_I2C::pulseEnable (uint8_t data)
{
   _i2cio.write (data | _En);   // En HIGH
   _i2cio.write (data & ~_En);  // En LOW
}