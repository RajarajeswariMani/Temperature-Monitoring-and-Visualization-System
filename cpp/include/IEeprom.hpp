#pragma once
#include <cstdint>
#include <string>

enum class HwRevision : uint8_t {
  RevA = 0, /* 1 degC / digit sensor   */
  RevB = 1  /* 0.1 degC / digit sensor */
};

struct HwConfig {
  HwRevision revision;
  std::string serialNumber; /* placeholder, e.g. "ABC1234" */
};

/* Hardware abstraction interface for the configuration EEPROM. */
class IEeprom {
public:
  virtual ~IEeprom() = default;
  virtual void init() = 0;
  virtual bool readConfig(HwConfig &outConfig) const = 0;
};
