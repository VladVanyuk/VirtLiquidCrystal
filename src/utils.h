#ifndef LCD_Utils_h
#define LCD_Utils_h

#include <Arduino.h>

#define DEBUG_LCD

#ifdef DEBUG_LCD

#define debugPrint(x) Serial.print(x)
#define debugPrintln(x) Serial.println(x)

#else
#define debugPrint(x) 
#define debugPrintln(x)
#endif //!  DEBUG_LCD

#endif //! LCD_Utils_h