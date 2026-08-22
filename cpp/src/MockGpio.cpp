#include "MockGpio.hpp"
#include <cstdio>

static const char *ledName(LedId led) {
  switch (led) {
  case LedId::Green:
    return "GREEN";
  case LedId::Yellow:
    return "YELLOW";
  case LedId::Red:
    return "RED";
  default:
    return "?";
  }
}

void MockGpio::setLed(LedId led, bool on) {
  size_t idx = static_cast<size_t>(led);
  if (idx >= state_.size()) {
    return;
  }
  if (state_[idx] != on) {
    state_[idx] = on;
    std::printf("  [GPIO] LED %-6s -> %s\n", ledName(led), on ? "ON" : "off");
  }
}

bool MockGpio::getLed(LedId led) const {
  size_t idx = static_cast<size_t>(led);
  return idx < state_.size() ? state_[idx] : false;
}
