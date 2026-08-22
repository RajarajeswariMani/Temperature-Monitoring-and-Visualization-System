#include "LedController.hpp"

LedController::LedController(IGpio &gpio) : gpio_(gpio) {}

void LedController::update(TempStatus status) {
  gpio_.setLed(LedId::Green, status == TempStatus::Ok);
  gpio_.setLed(LedId::Yellow, status == TempStatus::Warning);
  gpio_.setLed(LedId::Red, status == TempStatus::Critical);
}
