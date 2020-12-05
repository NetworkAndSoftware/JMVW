# JMVW Arduino library bundle

A set of library classes, which will be expanded over time

## Included classes

 ### Scheduler
 This is a cooperative multitasking scheduler that will allow you to write multiple loop functions, each with their own delays. This can greatly simplify writing code to do multiple things at once. On the ATMega328p it can intercept calls to the stock delay() function, so that even code that was not written to work with this library should play nice with it. 
 
 ### StdIO
  A class for standard io functionality. Provides printf method to print to the serial port.


Please take a look at the [JMVW Documentation](https://github.com/NetworkAndSoftware/JMVW/wiki)
