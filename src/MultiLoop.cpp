#include "MultiLoop.h"
#include <Arduino.h>
#include <limits.h>

MultiLoop::Loop *MultiLoop::loops[MultiLoop_MAX_LOOPS] = {};
int8_t MultiLoop::schedule[MultiLoop_MAX_LOOPS];
int8_t MultiLoop::count = 0;
MultiLoop::Priority MultiLoop::currentPriority = Normal; // we need to start with the lowest priority

MultiLoop::Loop::Loop(void (*fn)(), Priority priority)
{
  this->fn = fn;
  running = false;
  count = 0;
  totalTime = 0;
  this->priority = priority;
}

double MultiLoop::Loop::AverageRunTime() const
{
  if (0 == count)
    return 0;
  auto avg = (double)totalTime / (double)count;
  return avg;
}

void MultiLoop::Loop::Run()
{
  ++running;
  count++;
  lastStart = millis();
  (*fn)();
  auto end = millis();
  totalTime += (end - lastStart);
  running--;
}

bool MultiLoop::Loop::IsRunning()
{
  return 0 != running;
}

// track:
// - last launched
// - current priority
void MultiLoop::delay(unsigned long value)
{
  const auto oldMillis = ::millis();
  const auto deadline = oldMillis + value;

  Priority savedPriority = currentPriority;

  while (true)
  {
    auto millis = ::millis();
    if (millis > deadline)
      break; // time's up.

    auto timeLeft = deadline - millis;

    auto loop = FindNext(savedPriority, timeLeft);

    if (0 == loop)
      continue;

    currentPriority = loop->priority;
    loop->Run();
    currentPriority = savedPriority;
  }
}

// finds next candidate to run and places this at position 0 in schedule. unsuitable loops are moved up,
// so they will remain first in line after this one.
// any non running loop with priority higher than current will be run regardless of time left
// any with priority equal or lower than current will be run if we think we have time.
MultiLoop::Loop *MultiLoop::FindNext(Priority basePriority, unsigned long timeLeft)
{
  for (int8_t i = 0; i < count; i++)
  {
    const auto id = schedule[i];
    auto loop = loops[id];

    // is it suitable?
    if (!loop->IsRunning() && (loop->priority < basePriority || (loop->AverageRunTime() <= timeLeft)))
    {
      // id should go to last position and rest should shift left

      while (++i < count)
        schedule[i - 1] = schedule[i];

      schedule[count - 1] = id;

      return loop;
    }
  }
  return nullptr;
}

int8_t MultiLoop::AddLoop(void (*fn)(), Priority priority)
{
  for (int8_t i = 0; i < MultiLoop_MAX_LOOPS; i++)
  {
    if (loops[i] == nullptr)
    {
      loops[i] = new Loop(fn, priority);

      // now schedule it at the end.
      schedule[count++] = i;

      return i;
    }
  }
  return -1;
}

void MultiLoop::RemoveLoop(int8_t id)
{
  delete loops[id];
  loops[id] = nullptr;

  // find it in the schedule
  for (int8_t i = 0; i < count; i++)
  {
    if (schedule[i] == id)
    {
      count--;
      // shift remainder left
      for (auto j = i; j < count; j++)
        schedule[j] = schedule[j + 1];

      break;
    }
  }
}

void loop()
{
  MultiLoop::delay(ULONG_MAX >> 1);
}
