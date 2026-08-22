#pragma once
#include "IEeprom.hpp" // for HwRevision
#include <cstdint>
#include <memory>

/* Strategy pattern: encapsulates how a raw ADC digit is converted to
 * degrees Celsius. Each hardware revision fits a different sensor
 * type, so each gets its own strategy - adding a Rev-C sensor later
 * means adding one more class, no existing code changes. */
class ISensorResolutionStrategy {
public:
  virtual ~ISensorResolutionStrategy() = default;
  virtual float convert(uint16_t raw) const = 0;
};

/* Rev-A hardware: 1 degree Celsius per digit. */
class RevAResolutionStrategy final : public ISensorResolutionStrategy {
public:
  float convert(uint16_t raw) const override { return static_cast<float>(raw); }
};

/* Rev-B hardware: 0.1 degree Celsius per digit. */
class RevBResolutionStrategy final : public ISensorResolutionStrategy {
public:
  float convert(uint16_t raw) const override {
    return static_cast<float>(raw) * 0.1f;
  }
};

/* Factory: picks the correct strategy for the revision read from EEPROM. */
std::unique_ptr<ISensorResolutionStrategy>
makeSensorResolutionStrategy(HwRevision revision);
