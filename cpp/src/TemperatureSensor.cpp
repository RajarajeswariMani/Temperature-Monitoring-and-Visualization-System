#include "TemperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(
    IAdc &adc, std::unique_ptr<ISensorResolutionStrategy> strategy)
    : adc_(adc), strategy_(std::move(strategy)) {}

float TemperatureSensor::convert(uint16_t raw) const {
  return strategy_->convert(raw);
}

float TemperatureSensor::sampleCelsius() const {
  return convert(adc_.readRaw());
}
