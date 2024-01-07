#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <inttypes.h>
#include "VirtLiquidCrystal.h"

#define EXEC_TIME 37

#define DEFAULT_LINES 2
#define DEFAULT_COLS 16

#ifndef UINT8_MAX 
#define UINT8_MAX 0xff // 255
#endif // !UINT8_MAX 

class LiquidCrystal : public VirtLiquidCrystal
{
public:
  LiquidCrystal(uint8_t cols = DEFAULT_COLS, uint8_t lines = DEFAULT_LINES, uint8_t charsize = LCD_5x8DOTS,
                uint8_t bitmode = LCD_4BIT_MODE, uint8_t rs = 1 , uint8_t rw = UINT8_MAX, uint8_t enable = 0,
                 uint8_t d0 = 0, uint8_t d1 = 0, uint8_t d2 = 0, uint8_t d3 = 0,
                uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0,
                uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);

  ~LiquidCrystal();

  void init(uint8_t cols = DEFAULT_COLS, uint8_t lines = DEFAULT_LINES, uint8_t charsize = LCD_5x8DOTS,
            uint8_t bitmode = LCD_4BIT_MODE, uint8_t rs = 1, uint8_t rw = UINT8_MAX, uint8_t enable = 0,
            uint8_t d0 = 0, uint8_t d1 = 0, uint8_t d2 = 0, uint8_t d3 = 0,
            uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0,
            uint8_t backlighPin = 0, t_backlightPol pol = POSITIVE);

  void begin();

#if defined(ARDUINO_ARCH_ESP32)
  void analogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = UINT8_MAX);
#endif
  void setBacklightPin(uint8_t pin, t_backlightPol pol = POSITIVE);
  void setBacklight(uint8_t value);
   using Print::write;
private:
  void send(uint8_t value, uint8_t mode);
  // void write(uint8_t value); //todo remove write()
  void write4bits(uint8_t value);
  void write8bits(uint8_t value);
  void writeNbits(uint8_t value, uint8_t numBits);

  void pulseEnable();
  uint8_t _backlightPin;
  uint8_t _data_pins[8];
};

#endif