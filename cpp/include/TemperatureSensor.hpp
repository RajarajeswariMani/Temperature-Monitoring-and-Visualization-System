#pragma once
#include "IAdc.hpp"
#include "SensorResolutionStrategy.hpp"
#include <cstdint>
#include <memory>

/* Driver-level class: reads the ADC and converts to degrees Celsius
 * using whatever resolution strategy matches the fitted hardware
 * revision. Depends only on interfaces (IAdc, ISensorResolutionStrategy)
 * so it is fully testable without real hardware. */
class TemperatureSensor {
public:
  TemperatureSensor(IAdc &adc,
                    std::unique_ptr<ISensorResolutionStrategy> strategy);

  float sampleCelsius() const;
  float convert(uint16_t raw) const;

private:
  IAdc &adc_;
  std::unique_ptr<ISensorResolutionStrategy> strategy_;
};
