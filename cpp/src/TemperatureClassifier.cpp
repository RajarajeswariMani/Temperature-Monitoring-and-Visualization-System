#include "TemperatureClassifier.hpp"

TempStatus TemperatureClassifier::classify(float tempC) {
  if (tempC >= kCriticalHiC || tempC < kCriticalLoC) {
    return TempStatus::Critical;
  }
  if (tempC >= kWarningC) {
    return TempStatus::Warning;
  }
  return TempStatus::Ok;
}
