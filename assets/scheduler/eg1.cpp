// pull in the Scheduler class
#include "Scheduler.h"

// brings jmvw namespace into scope
using namespace jmvw;

void setup()
{ // set up three independent loops
  Scheduler::AddLoop(LoopSwitch);
  Scheduler::AddLoop(LoopTemperature);
  Scheduler::AddLoop(LoopDisplay);
}

// read switch status 10 times/second
void LoopSwitch()
{
  if (digitalRead(pinSwitch))
  {
    // debounce at leisure
    while (digitalRead(pinSwitch))
      delay(100);

    swStatus = true;
    return;
  }
  delay(100);
  return;
}

// update average 100 times/second
void LoopTemperature()
{
  UpdateAverage();
  delay(10);
}

// update display 4 times/secound
void LoopDisplay()
{
  ShowStatus();
  delay(250);
}

// do not include the normal loop() function