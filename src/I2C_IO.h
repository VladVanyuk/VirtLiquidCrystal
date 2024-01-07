
#ifndef _I2CIO_H_
#define _I2CIO_H_

#if (ARDUINO < 100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#if (ARDUINO < 10000)
#include <../Wire/Wire.h>
#else
#include <Wire.h>
#endif
#include <inttypes.h>

#include "utils.h"

#define I2C_NO_ADDR 0x0
#define I2C_NO_MASK 0xFF
#define I2C_NO_SHADOW 0x0

/*!
 @class
 @brief    I2C_IO
 @note  Library driver to control PCF8574 based ASICs. Implementing
 library calls to set/get port through I2C bus.
 */

class I2C_IO
{
public:
   
   I2C_IO(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW);

   //~I2C_IO();

   uint8_t init(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW);

   uint8_t begin();

   void pinMode(uint8_t pin, uint8_t dir);

   void portMode(uint8_t dir);

   uint8_t read(void);

   uint8_t digitalRead(uint8_t pin);

   uint8_t writeExpander(uint8_t value);

   uint8_t digitalWrite(uint8_t pin, uint8_t level);

private:
   uint8_t _pinShadow;   // Shadow output
   uint8_t _dirMask;    // Direction mask
   uint8_t _i2cAddr;    // I2C address
   bool _initialised;   // Initialised object

   bool isAvailable(uint8_t i2cAddr);
};

#endif