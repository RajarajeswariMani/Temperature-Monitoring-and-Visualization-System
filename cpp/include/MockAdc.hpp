#pragma once
#include "IAdc.hpp"

/* PC-demo mock: returns whatever value the test harness has set,
 * standing in for a real ADC peripheral driver. */
class MockAdc final : public IAdc {
public:
  void init() override { rawValue_ = 0u; }
  uint16_t readRaw() const override { return rawValue_; }

  void setMockValue(uint16_t value) { rawValue_ = value; }

private:
  uint16_t rawValue_ = 0u;
};
