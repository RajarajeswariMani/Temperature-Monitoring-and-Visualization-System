#pragma once
#include "LedController.hpp"
#include "TemperatureSensor.hpp"
#include <atomic>
#include <cstdint>

/* Application-layer orchestrator.
 *
 * onTimerTick() plays the role of the ISR: it is the ONLY method that
 * would ever run in interrupt context on real hardware, so - just
 * like isr.c in the C version - it does the absolute minimum (latch
 * raw ADC value, set a flag) to keep the 100us period jittery-free.
 *
 * processPendingSample() runs in normal (thread/main-loop) context and
 * does the conversion, classification and LED update. */
class TemperatureMonitor {
public:
  TemperatureMonitor(IAdc &adc, TemperatureSensor &sensor,
                     LedController &ledController);

  /* ISR-context: minimal work only. */
  void onTimerTick();

  bool isSampleReady() const;

  /* Main-loop context: heavier work deferred out of the ISR. */
  void processPendingSample();

  float lastTemperatureC() const { return lastTemperatureC_; }
  TempStatus lastStatus() const { return lastStatus_; }

private:
  IAdc &adc_;
  TemperatureSensor &sensor_;
  LedController &ledController_;

  volatile uint16_t latchedRaw_ = 0u;
  std::atomic<bool> sampleReady_{false};

  float lastTemperatureC_ = 0.0f;
  TempStatus lastStatus_ = TempStatus::Ok;
};
