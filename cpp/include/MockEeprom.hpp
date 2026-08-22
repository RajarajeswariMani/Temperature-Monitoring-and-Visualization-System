#pragma once
#include "IEeprom.hpp"

/* PC-demo mock: returns a fixed/settable configuration instead of
 * reading a real I2C EEPROM. */
class MockEeprom final : public IEeprom {
public:
  void init() override {}
  bool readConfig(HwConfig &outConfig) const override;

  void setMockConfig(HwRevision revision, const std::string &serial);

private:
  HwConfig config_{HwRevision::RevA, "ABC1234"};
};
