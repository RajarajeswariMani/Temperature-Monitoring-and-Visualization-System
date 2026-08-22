#pragma once
#include "IGpio.hpp"
#include <array>
#include <cstddef>

/* PC-demo mock: prints LED transitions to stdout instead of driving
 * real GPIO pins. */
class MockGpio final : public IGpio {
public:
  void init() override { state_.fill(false); }

  void setLed(LedId led, bool on) override;
  bool getLed(LedId led) const override;

private:
  std::array<bool, static_cast<size_t>(LedId::Count)> state_{};
};
