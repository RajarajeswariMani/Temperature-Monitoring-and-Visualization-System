#include "TimerIsr.hpp"
#include "TemperatureMonitor.hpp"

static TemperatureMonitor *s_monitor = nullptr;

void TimerIsr_RegisterMonitor(TemperatureMonitor *monitor) {
  s_monitor = monitor;
}

/* This is the function that would actually be placed in the vector
 * table on real hardware (extern "C" so its symbol/linkage matches
 * what a C-based startup/vector-table file expects). */
extern "C" void isr_timer_100us_handler(void) {
  if (s_monitor != nullptr) {
    s_monitor->onTimerTick();
  }
}
