# JMVW Arduino library bundle

A set of library classes, which will be expanded over time

## Included classes

 - Scheduler
 This is cooperative multitasking scheduler that will allow you to write multiple loop functions, each with their own delays. This can greatly simplify writing code to do multiple things at once. On the ATMega328p it can intercept calls to the stock delay() function, so that even code that was not written to work with this library should play nice with it. 
 
 - StdIO
  A class for standard io functionality. Provides printf method to print to the serial port.


## Usage

First clone the library by downloading the zip file and unpacking it in your library directory.

The library uses a C++ namespace to avoid name collisions with other libraries. The name of the namespace is jmvw. You will have to either preface all references to classes in this library bundle with jmvw::, or you can use a using statement at the start of your code.

