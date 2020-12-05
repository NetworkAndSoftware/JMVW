// pull in the Scheduler class
#include "Scheduler.h"

// brings jmvw namespace into scope
using namespace jmvw;

void setup()
{ 
  pinMode(A0, INPUT);
  auto value = analogRead(A0);

  StdIO.printf("Read %d from pin A0.", input);
}