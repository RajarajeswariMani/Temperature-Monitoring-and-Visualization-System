#include "MockEeprom.hpp"

bool MockEeprom::readConfig(HwConfig &outConfig) const {
  outConfig = config_;
  return true;
}

void MockEeprom::setMockConfig(HwRevision revision, const std::string &serial) {
  config_.revision = revision;
  config_.serialNumber = serial;
}
