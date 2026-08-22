#include "SensorResolutionStrategy.hpp"

std::unique_ptr<ISensorResolutionStrategy>
makeSensorResolutionStrategy(HwRevision revision) {
  switch (revision) {
  case HwRevision::RevA:
    return std::make_unique<RevAResolutionStrategy>();

  case HwRevision::RevB:
    return std::make_unique<RevBResolutionStrategy>();

  default:
    return nullptr;
  }
}
