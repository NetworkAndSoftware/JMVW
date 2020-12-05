#include "Scheduler.h"
#include <Arduino.h>
#include <limits.h>
#include "freeMemory.h"

namespace jmvw
{
  Scheduler::Loop* Scheduler::loops[SCHEDULER_MAX_LOOPS] = {};
  int8_t Scheduler::schedule[SCHEDULER_MAX_LOOPS];
  int8_t Scheduler::count = 0;
  Scheduler::Priority Scheduler::currentPriority = Normal; // we need to start with the lowest priority

  Scheduler::Loop::Loop(void (*fn)(), Priority priority)
  {
    this->fn = fn;
    running = false;
    count = 0;
    totalTime = 0;
    this->priority = priority;
  }

  double Scheduler::Loop::AverageRunTime() const
  {
    if (0 == count)
      return 0;
    auto avg = (double)totalTime / (double)count;
    return avg;
  }

  void Scheduler::Loop::Run()
  {
    ++running;
    count++;
    lastStart = millis();
    (*fn)();
    auto end = millis();
    totalTime += (end - lastStart);
    running--;
  }

  bool Scheduler::Loop::IsRunning()
  {
    return 0 != running;
  }

  // track:
  // - last launched
  // - current priority
  void Scheduler::delay(unsigned long value)
  {
    const auto oldMillis = ::millis();
    const auto deadline = oldMillis + value;

    Priority savedPriority = currentPriority;

    //Serial.println(::freeRam());
    while (true)
    {
      auto millis = ::millis();
      if (millis > deadline)
        break; // time's up.

      auto timeLeft = deadline - millis;

      auto loop = FindNext(savedPriority, timeLeft);
      //Serial.println();

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
  Scheduler::Loop* Scheduler::FindNext(Priority basePriority, unsigned long timeLeft)
  {
    //Serial.println("findNext");
    for (int8_t i = 0; i < count; i++)
    {
      const auto id = schedule[i];
      //Serial.print(id);
      //Serial.print(' ');
      auto loop = loops[id];

      // is it suitable?
      if (!loop->IsRunning() && (loop->priority < basePriority || (loop->AverageRunTime() <= timeLeft)))
      {
        // id should go to last position and rest should shift left

        while (++i < count)
          schedule[i - 1] = schedule[i];

        schedule[count - 1] = id;

        //Serial.println(id);
        return loop;
      }
    }
    return nullptr;
  }

  int8_t Scheduler::AddLoop(void (*fn)(), Priority priority)
  {
    for (int8_t i = 0; i < SCHEDULER_MAX_LOOPS; i++)
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

  void Scheduler::RemoveLoop(int8_t id)
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
} // namespace jmvw

void loop()
{
  jmvw::Scheduler::delay(ULONG_MAX >> 1);
}

volatile unsigned long *_unsigned_long_regs_r8_11 = (unsigned long *)8;
volatile unsigned long *_unsigned_long_regs_r12_15 = ( unsigned long *)12;
unsigned long _ul1, _ul2;
volatile unsigned long *ms = nullptr;

void yield(void)
{
  // unfortunately, yield did not get a parameter to tell us how much time we have.
  // we will have to play dirty and read the ms parameter to our caller by grabbing
  // from the register that we know was used by the compiler.
  // Obviously this is platform dependent.

  // save memory mapped registers
  _ul1 = *_unsigned_long_regs_r8_11;
  _ul2 = *_unsigned_long_regs_r12_15;

  const unsigned long bigprime = 4294967291;

  // try to figure out which registers are used by the caller.
  if (nullptr == ms)
  {
    if (bigprime == _ul1)
    { ms = &_ul1;
      *_unsigned_long_regs_r8_11 = 1; // force rest of long delay to 1
      return;
    }
    else if (bigprime == _ul2)
    { ms = &_ul2;
      *_unsigned_long_regs_r12_15 = 1; // force rest of long delay to 1
      return;
    }
    delay(bigprime);
    if (nullptr == ms)
    {
      Serial.println("failure to find caller's parameter in yield");
      while (1);
    }
  }

  if (1 != *ms) // we get some calls from delay loop that we caused ourselfs
    jmvw::Scheduler::delay(*ms);
}
