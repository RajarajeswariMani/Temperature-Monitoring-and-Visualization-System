#include "TemperatureMonitor.hpp"
#include "TemperatureClassifier.hpp"

TemperatureMonitor::TemperatureMonitor(IAdc &adc, TemperatureSensor &sensor,
                                       LedController &ledController)
    : adc_(adc), sensor_(sensor), ledController_(ledController) {}

void TemperatureMonitor::onTimerTick() {
  /* ISR-context: minimal, deterministic work only. */
  latchedRaw_ = adc_.readRaw();
  sampleReady_.store(true, std::memory_order_release);
}

bool TemperatureMonitor::isSampleReady() const {
  return sampleReady_.load(std::memory_order_acquire);
}

void TemperatureMonitor::processPendingSample() {
  if (!isSampleReady()) {
    return;
  }
  uint16_t raw = latchedRaw_;
  sampleReady_.store(false, std::memory_order_release);

  lastTemperatureC_ = sensor_.convert(raw);
  lastStatus_ = TemperatureClassifier::classify(lastTemperatureC_);
  ledController_.update(lastStatus_);
}
