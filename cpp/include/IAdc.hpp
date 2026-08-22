#pragma once
#include <cstdint>

/* Hardware abstraction interface for the ADC peripheral. */
class IAdc {
public:
  virtual ~IAdc() = default;
  virtual void init() = 0;
  virtual uint16_t readRaw() const = 0;
};
