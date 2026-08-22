#pragma once

/* Real hardware timer ISRs are plain C-linkage functions registered
 * in the vector table; they cannot be C++ member functions directly.
 * This trampoline bridges that gap: register a TemperatureMonitor
 * instance once at startup, then the vector table (or, on PC, the
 * demo harness) calls this free function on every timer tick. */
class TemperatureMonitor;

void TimerIsr_RegisterMonitor(TemperatureMonitor *monitor);

extern "C" void isr_timer_100us_handler(void);
