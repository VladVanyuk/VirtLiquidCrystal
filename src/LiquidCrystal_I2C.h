#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include "I2C_IO.h"
#include "VirtLiquidCrystal.h"

//#define EN B00000100 // Enable bit
//#define RW B00000010 // Read/Write bit
//#define RS B00000001 // Register select bit

#define LCD_EN 6  // Enable bit
#define LCD_RW 5  // Read/Write bit
#define LCD_RS 4  // Register select bit

#define LCD_D4 0
#define LCD_D5 1
#define LCD_D6 2
#define LCD_D7 3

#define LCD_DEFAULT_ADDR 0x27 // Default I2C address
#define LCD_DEFAULT_COLS 20
#define LCD_DEFAULT_ROWS 4

class LiquidCrystal_I2C : public VirtLiquidCrystal, public I2C_IO
{
public:

    LiquidCrystal_I2C(uint8_t lcd_addr = LCD_DEFAULT_ADDR, uint8_t lcd_cols = LCD_DEFAULT_COLS, uint8_t lcd_rows = LCD_DEFAULT_ROWS);

   // ~LiquidCrystal_I2C();

    uint8_t init(uint8_t lcd_addr = LCD_DEFAULT_ADDR, uint8_t lcd_cols = LCD_DEFAULT_COLS, uint8_t lcd_rows = LCD_DEFAULT_ROWS,
                      uint8_t charsize = LCD_5x8DOTS, uint8_t En = LCD_EN, uint8_t Rw = LCD_RW, uint8_t Rs = LCD_RS,
                      uint8_t d4 = LCD_D4, uint8_t d5 = LCD_D5, uint8_t d6 = LCD_D6, uint8_t d7 = LCD_D7,
                      uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);

    void begin();

    void setBacklightPin(uint8_t pin, t_backlightPol pol = POSITIVE);
    void setBacklight(uint8_t new_val);

    uint8_t getBacklight();

    //void load_custom_character(uint8_t char_num, uint8_t *rows); // alias for createChar()
    void printstr(const char[]);

private:
    void config (uint8_t En, uint8_t Rw, uint8_t Rs, 
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                    uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);
    void send(uint8_t value, uint8_t mode);
    void write4bits(uint8_t value, uint8_t mode);
    // uint8_t write(uint8_t);
    void pulseEnable(uint8_t);

    // using I2C_IO::write;

    uint8_t _backlightPinMask; // Backlight IO pin mask
    uint8_t _backlightStsMask; // Backlight status mask

    uint8_t _data_pins[4]; // LCD data lines
};

#endif // LiquidCrystal_I2C_h