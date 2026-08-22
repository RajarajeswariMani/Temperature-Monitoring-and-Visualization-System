/* PC demonstration harness for the C++ / OOP temperature monitoring
 * firmware. As with the C version, this main() only exists to drive
 * the architecture on a PC using mocked HAL objects; on target,
 * isr_timer_100us_handler() would be wired into the interrupt vector
 * table and there would be no such loop. */

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "LedController.hpp"
#include "MockAdc.hpp"
#include "MockEeprom.hpp"
#include "MockGpio.hpp"
#include "SensorResolutionStrategy.hpp"
#include "TemperatureMonitor.hpp"
#include "TemperatureSensor.hpp"
#include "TimerIsr.hpp"

namespace {

struct DemoPoint {
  const char *label;
  uint16_t rawRevA;
  uint16_t rawRevB;
};

const char *statusToString(TempStatus status) {
  switch (status) {
  case TempStatus::Ok:
    return "OK";
  case TempStatus::Warning:
    return "WARNING";
  case TempStatus::Critical:
    return "CRITICAL";
  default:
    return "?";
  }
}

void runDemoForRevision(MockAdc &adc, MockGpio &gpio, MockEeprom &eeprom,
                        HwRevision revision, const std::string &serial) {
  static const std::vector<DemoPoint> points = {
      {"OK (low-normal)", 20, 200},    {"OK (high-normal)", 60, 600},
      {"WARNING (>=85C)", 90, 900},    {"CRITICAL (>=105C)", 110, 1100},
      {"CRITICAL (<5C, cold)", 2, 20}, {"back to OK", 25, 250},
  };

  eeprom.setMockConfig(revision, serial);

  HwConfig cfg;
  if (!eeprom.readConfig(cfg)) {
    std::printf("Failed to read EEPROM configuration!\n");
    return;
  }

  auto strategy = makeSensorResolutionStrategy(cfg.revision);

  if (!strategy) {
    std::printf("Invalid hardware revision!\n");
    return;
  }

  TemperatureSensor sensor(adc, std::move(strategy));
  LedController ledController(gpio);
  TemperatureMonitor monitor(adc, sensor, ledController);

  TimerIsr_RegisterMonitor(&monitor);

  std::printf("\n=== Hardware Revision: %s | Serial: %s ===\n",
              cfg.revision == HwRevision::RevA ? "Rev-A (1 degC/digit)"
                                               : "Rev-B (0.1 degC/digit)",
              cfg.serialNumber.c_str());

  for (const auto &point : points) {
    uint16_t raw =
        (cfg.revision == HwRevision::RevA) ? point.rawRevA : point.rawRevB;

    adc.setMockValue(raw);
    isr_timer_100us_handler(); /* simulate one 100us timer tick */

    monitor.processPendingSample();

    std::printf("[%-24s] raw=%-5u temp=%6.1fC status=%s\n", point.label, raw,
                static_cast<double>(monitor.lastTemperatureC()),
                statusToString(monitor.lastStatus()));
  }

  TimerIsr_RegisterMonitor(nullptr);
}

} // namespace

int main() {
  std::printf("Temperature Monitoring & Visualization - C++ (OOP) demo\n");

  MockAdc adc;
  MockGpio gpio;
  MockEeprom eeprom;

  adc.init();
  gpio.init();
  eeprom.init();

  runDemoForRevision(adc, gpio, eeprom, HwRevision::RevA, "ABC1234");
  runDemoForRevision(adc, gpio, eeprom, HwRevision::RevB, "XYZ9876");

  return 0;
}
