#ifndef _StdIO_H
#define _StdIO_H

// Define PRINT_BUFFER_SIZE before including this file to change the default size
#ifndef PRINT_BUFFER_SIZE
#define PRINT_BUFFER_SIZE 256
#endif

namespace jmvw
{
  class StdIO
  {
  public:
    // normally bad programming practic, but because of our extreme memory limitations, allow access to printBuffer for reuse
    static char printBuffer[];

    // prints to the buffer and returns a pointer to the resulting string
    static char *sprintf(const char *format, ...);

    // prints to serial port
    static int printf(const char *format, ...);
  };
} // namespace jmvw
#endif