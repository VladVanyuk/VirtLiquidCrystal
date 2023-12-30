// ---------------------------------------------------------------------------
// Created by Francisco Malpartida on 20/08/11.
// Copyright 2011 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no
// warranty, express or implied, as to its usefulness for any purpose.
//
// Thread Safe: No
// Extendable: Yes
//
// @file I2CIO.h
// This file implements a basic IO library using the PCF8574 I2C IO Expander
// chip.
//
// @brief
// Implement a basic IO library to drive the PCF8574* I2C IO Expander ASIC.
// The library implements basic IO general methods to configure IO pin direction
// read and write uint8_t operations and basic pin level routines to set or read
// a particular IO port.
//
//
// @version API 1.0.0
//
// @author F. Malpartida - fmalpartida@gmail.com
// ---------------------------------------------------------------------------

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

#include "I2CIO.h"


// CLASS VARIABLES
// ---------------------------------------------------------------------------

// CONSTRUCTOR
// ---------------------------------------------------------------------------
I2CIO::I2CIO(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW)
{
   init(i2cAddr, dirMask, pinShadow);
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------
uint8_t I2CIO::init(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW)
{
   _initialised = isAvailable(_i2cAddr);
   if (!_initialised)
   {
      _i2cAddr = i2cAddr;
      _dirMask = dirMask; // mark all as INPUTs
      _pinShadow = pinShadow;   // no values set
   }
   return _initialised;
}

uint8_t I2CIO::begin()
{
   Wire.begin();

   _initialised = isAvailable(_i2cAddr);

   if (_initialised)
   {
#if (ARDUINO < 100)
      _pinShadow = Wire.receive();
#else
      _pinShadow = Wire.read(); // Remove the byte read don't need it.
#endif
   }
   return (_initialised);
}


void I2CIO::pinMode(uint8_t pin, uint8_t dir)
{
   if (_initialised)
   {
      if (OUTPUT == dir)
      {
         _dirMask &= ~(1 << pin);
      }
      else
      {
         _dirMask |= (1 << pin);
      }
   }
}


void I2CIO::portMode(uint8_t dir)
{

   if (_initialised)
   {
      if (dir == INPUT)
      {
         _dirMask = 0xFF;
      }
      else
      {
         _dirMask = 0x00;
      }
   }
}


uint8_t I2CIO::read(void)
{
   uint8_t retVal = 0;

   if (_initialised)
   {
      Wire.requestFrom(_i2cAddr, (uint8_t)1);
#if (ARDUINO < 100)
      retVal = (_dirMask & Wire.receive());
#else
      retVal = (_dirMask & Wire.read());
#endif
   }
   return (retVal);
}


uint8_t I2CIO::write(uint8_t value)
{
   uint8_t status = 0;

   if (_initialised)
   {
      // Only write HIGH the values of the ports that have been initialised as
      // outputs updating the output shadow of the device
      _pinShadow = (value & ~(_dirMask));

      Wire.beginTransmission(_i2cAddr);
#if (ARDUINO < 100)
      Wire.send(_pinShadow);
#else
      Wire.write(_pinShadow);
#endif
      status = Wire.endTransmission();
   }
   return ((status == 0));
}


uint8_t I2CIO::digitalRead(uint8_t pin)
{
   uint8_t pinVal = 0;

   // Check if initialised and that the pin is within range of the device
   // -------------------------------------------------------------------
   if ((_initialised) && (pin <= 7))
   {
      // Remove the values which are not inputs and get the value of the pin
      pinVal = this->read() & _dirMask;
      pinVal = (pinVal >> pin) & 0x01; // Get the pin value
   }
   return (pinVal);
}


uint8_t I2CIO::digitalWrite(uint8_t pin, uint8_t level)
{
   uint8_t writeVal;
   uint8_t status = 0;

   // Check if initialised and that the pin is within range of the device
   // -------------------------------------------------------------------
   if ((_initialised) && (pin <= 7))
   {
      // Only write to HIGH the port if the port has been configured as
      // an OUTPUT pin. Add the new state of the pin to the shadow
      writeVal = (1 << pin) & ~_dirMask;
      if (level == HIGH)
      {
         _pinShadow |= writeVal;
      }
      else
      {
         _pinShadow &= ~writeVal;
      }
      status = this->write(_pinShadow);
   }
   return (status);
}

//
// PRIVATE METHODS
// ---------------------------------------------------------------------------
bool I2CIO::isAvailable(uint8_t i2cAddr)
{
   int ret;

   Wire.beginTransmission(i2cAddr);
   ret = Wire.endTransmission();

   return (ret == 0) ? true : false;
   
}
