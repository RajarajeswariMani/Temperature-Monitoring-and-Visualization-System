#pragma once
#include "IGpio.hpp"
#include "TemperatureClassifier.hpp"

/* Drives exactly one of the three status LEDs according to a
 * TempStatus classification. Depends only on IGpio. */
class LedController {
public:
  explicit LedController(IGpio &gpio);

  void update(TempStatus status);

private:
  IGpio &gpio_;
};
