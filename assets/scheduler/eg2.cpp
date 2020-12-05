// pull in the Scheduler class
#include "Scheduler.h"

// brings jmvw namespace into scope
using namespace jmvw;

void setup()
{ // set up three independent loops
  Scheduler::AddLoop(Loop1);
  Scheduler::AddLoop(Loop2);
}

// do this 10 times/second
void Loop1()
{
  DoSomething();
  delay(100);
  return;
}

// do this once a second
void Loop2()
{
  DoSomethingElse();
  delay(1000); 
}

// do not include the normal loop() function