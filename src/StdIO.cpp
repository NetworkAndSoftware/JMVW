#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>
#include "StdIO.h"

namespace jmvw
{
  char StdIO::printBuffer[PRINT_BUFFER_SIZE];

  int StdIO::printf(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    int r = vsnprintf(printBuffer, sizeof printBuffer, format, args);
    Serial.print(printBuffer);
    return r;
  }

  char *StdIO::sprintf(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    int r = snprintf(printBuffer, sizeof printBuffer, format, args);
    return printBuffer;
  }
} // namespace jmvw