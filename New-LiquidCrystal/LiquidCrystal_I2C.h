
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h
#include <inttypes.h>
#include <Print.h>

#include "I2CIO.h"
#include "LCD.h"


class LiquidCrystal_I2C : public LCD
{
public:


   LiquidCrystal_I2C (uint8_t lcd_Addr);

   LiquidCrystal_I2C (uint8_t lcd_Addr, uint8_t backlighPin, t_backlightPol pol = POSITIVE);

   
   LiquidCrystal_I2C( uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs);
 
   LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                     uint8_t backlighPin, t_backlightPol pol = POSITIVE);

   
   LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );
   
   LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);
   
   virtual void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  
   virtual void send(uint8_t value, uint8_t mode);

  
   void setBacklightPin ( uint8_t pin, t_backlightPol pol );

  
   void setBacklight ( uint8_t value );

  void config (uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
               uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
               uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);

private:
   int  init();
   void write4bits(uint8_t value, uint8_t mode);
   void pulseEnable(uint8_t);

   uint8_t _Addr;             // I2C Address of the IO expander
   uint8_t _backlightPinMask; // Backlight IO pin mask
   uint8_t _backlightStsMask; // Backlight status mask

   I2CIO   _i2cio;            // I2CIO PCF8574* expansion module driver I2CLCDextraIO

   uint8_t _En;               // LCD expander word for enable pin
   uint8_t _Rw;               // LCD expander word for R/W pin
   uint8_t _Rs;               // LCD expander word for Register Select pin
   uint8_t _data_pins[4];     // LCD data lines
};

#endif
