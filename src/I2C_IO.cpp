
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

#include "I2C_IO.h"


I2C_IO::I2C_IO(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW)
{
   init(i2cAddr, dirMask, pinShadow);
}

uint8_t I2C_IO::init(uint8_t i2cAddr = I2C_NO_ADDR, uint8_t dirMask = I2C_NO_MASK, uint8_t pinShadow = I2C_NO_SHADOW)
{
   _i2cAddr = i2cAddr;
   _dirMask = dirMask;
   _pinShadow = pinShadow; 
   _initialised = isAvailable(_i2cAddr);
   return _initialised;
}

uint8_t I2C_IO::begin()
{
   Wire.begin();

   _initialised = isAvailable(_i2cAddr);

   if (_initialised)
   {
#if (ARDUINO < 100)
      Serial.println("rec");
      _pinShadow = Wire.receive();
#else
      Serial.println("read");
      _pinShadow = Wire.read(); // Remove the byte read don't need it.
#endif
   
      portMode ( OUTPUT );
      writeExpander(LOW);
   }
   return (_initialised);
}


void I2C_IO::pinMode(uint8_t pin, uint8_t dir)
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


void I2C_IO::portMode(uint8_t dir)
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


uint8_t I2C_IO::read(void)
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


uint8_t I2C_IO::writeExpander(uint8_t value)
{
   uint8_t status = 0;

   if (_initialised)
   {
      // Only writeExpander HIGH the values of the ports that have been initialised as
      // outputs updating the output shadow of the device
      _pinShadow = (value & ~(_dirMask)); //_shadow = ( value | _dirMask );

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


uint8_t I2C_IO::digitalRead(uint8_t pin)
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


uint8_t I2C_IO::digitalWrite(uint8_t pin, uint8_t level)
{
   uint8_t writeVal;
   uint8_t status = 0;

   // Check if initialised and that the pin is within range of the device
   // -------------------------------------------------------------------
   if ((_initialised) && (pin <= 7))
   {
      // Only writeExpander to HIGH the port if the port has been configured as
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
      status = this->writeExpander(_pinShadow);
   }
   return (status);
}

//
// PRIVATE METHODS
// ---------------------------------------------------------------------------
bool I2C_IO::isAvailable(uint8_t i2cAddr)
{
   int ret;

   Wire.beginTransmission(i2cAddr);
   ret = Wire.endTransmission();

   return (ret == 0) ? true : false; //false if err
   
}
