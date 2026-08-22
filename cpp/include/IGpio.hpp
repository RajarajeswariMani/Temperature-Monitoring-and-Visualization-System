#pragma once

enum class LedId { Green = 0, Yellow, Red, Count };

/* Hardware abstraction interface for the GPIO-driven LEDs. */
class IGpio {
public:
  virtual ~IGpio() = default;
  virtual void init() = 0;
  virtual void setLed(LedId led, bool on) = 0;
  virtual bool getLed(LedId led) const = 0;
};
