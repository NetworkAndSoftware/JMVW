# JMVW Arduino library bundle

A library bundle, expected to grow over time.

## Table of contents

  1. [Included classes](#included-classes)
  2. [Scheduler](#scheduler)
  3. [StdIO](#stdio)
  4. [Documentation](https://github.com/NetworkAndSoftware/JMVW/wiki)
   


## Included classes

 ### Scheduler
 This is a cooperative multitasking scheduler that will allow you to write multiple loop functions, each with their own delays. This can greatly simplify writing code to do multiple things at once. On the ATMega328p it can intercept calls to the stock delay() function, so that even code that was not written to work with this library should play nice with it. 
 
 ![Example](assets/scheduler/eg2.svg)

 ### StdIO
  A class for standard io functionality. Provides printf method to print to the serial port.

![Example](assets/stdio/eg1.svg)


## [Documentation](https://github.com/NetworkAndSoftware/JMVW/wiki)
