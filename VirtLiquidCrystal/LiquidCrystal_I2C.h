#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

#include <inttypes.h>
#include <Print.h>
#include <LCD_BaseClass.h>

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */
class LiquidCrystal_I2C : public VirtLiquidCrystal {
public:
 /**
 * Constructor
 *
 * @param lcd_addr I2C slave address of the LCD display. Most likely printed on the
 * LCD circuit board, or look in the supplied LCD documentation.
 * @param lcd_cols Number of columns your LCD display has.
 * @param lcd_rows Number of rows your LCD display has.
 * @param charsize The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
 */
 LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);

 /**
 * Set the LCD display in the correct begin state, must be called before anything else is done.
 */
 void begin();

 void noBacklight();
 void backlight();
 bool getBacklight();

 inline void blink_on() { blink(); }
 inline void blink_off() { noBlink(); }
 inline void cursor_on() { cursor(); }
 inline void cursor_off() { noCursor(); }

// Compatibility API function aliases
 void setBacklight(uint8_t new_val); // alias for backlight() and nobacklight()
 void load_custom_character(uint8_t char_num, uint8_t *rows); // alias for createChar()
 void printstr(const char[]);

private:
 void send(uint8_t, uint8_t);
 void write4bits(uint8_t);
 void expanderWrite(uint8_t);
 void pulseEnable(uint8_t);
 uint8_t _addr;
 uint8_t _cols;
 uint8_t _rows;
 uint8_t _charsize;
 uint8_t _backlightval;
};

#endif // FDB_LIQUID_CRYSTAL_I2C_H