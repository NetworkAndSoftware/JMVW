#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdint.h>

#ifndef SCHEDULER_MAX_LOOPS
#define SCHEDULER_MAX_LOOPS 8
#endif

namespace jmvw
{
  // A single threaded cooperative multitasking "loop" scheduler.
  // will not work with multi-threading.

  class Scheduler
  {
  public:
    enum Priority
    {
      Low = 0x1000,
      Normal = 0x100,
      High = 0x80,
      Critical = 0x10
    };

    static void delay(unsigned long value);
    static int8_t AddLoop(void (*fn)(), Priority priority = Priority::Normal);
    static void RemoveLoop(int8_t id);

  private:
    class Loop
    {
    public:
      Loop(void (*fn)(), Priority Priority);
      double AverageRunTime() const;
      void Run();
      bool IsRunning();

      Priority priority;

    private:
      void (*fn)();

      int8_t running;
      unsigned long totalTime;
      unsigned long count;
      unsigned long lastStart;
    };

    static Loop *FindNext(Priority basePriority, unsigned long timeLeft);

    static Loop *loops[SCHEDULER_MAX_LOOPS];
    static int8_t schedule[SCHEDULER_MAX_LOOPS];
    static int8_t count;
    static Priority currentPriority;
  };

} // namespace jmvw
#endif