/*
 * Unit tests for the C++ / OOP implementation, using a small
 * self-contained header-only framework with no external dependency.
 *
 * The tests exercise the real production classes through mocked hardware
 * backends. No target hardware or emulator is required.
 *
 * The automated tests cover the applicable functional test cases:
 * TC-01 through TC-08, TC-10 and TC-11.
 *
 * TC-09, TC-12, TC-13, TC-14 and TC-15 are verified separately where
 * applicable through code inspection, C-specific error handling,
 * build/execution verification, C/C++ behavioural comparison and
 * compiler-warning verification as defined in TEST_PLAN.md.
 */

#include "cpp_test_framework.hpp"

#include "LedController.hpp"
#include "MockAdc.hpp"
#include "MockEeprom.hpp"
#include "MockGpio.hpp"
#include "SensorResolutionStrategy.hpp"
#include "TemperatureClassifier.hpp"
#include "TemperatureMonitor.hpp"
#include "TemperatureSensor.hpp"
#include "TimerIsr.hpp"

/* TC-01 */
static void test_revA_conversion()
{
  TEST_BEGIN("TC-01 Rev-A raw->Celsius (x1)");

  RevAResolutionStrategy strategy;

  ASSERT_EQ(strategy.convert(10), 10.0f,
            "raw=10 should be 10.0C");

  ASSERT_EQ(strategy.convert(85), 85.0f,
            "raw=85 should be 85.0C");

  TEST_PASS();
}

/* TC-02 */
static void test_revB_conversion()
{
  TEST_BEGIN("TC-02 Rev-B raw->Celsius (x0.1)");

  RevBResolutionStrategy strategy;

  float t1 = strategy.convert(100);
  float t2 = strategy.convert(850);

  ASSERT_TRUE(t1 > 9.99f && t1 < 10.01f,
              "raw=100 should be 10.0C");

  ASSERT_TRUE(t2 > 84.99f && t2 < 85.01f,
              "raw=850 should be 85.0C");

  TEST_PASS();
}

/* TC-03 */
static void test_ok_warning_boundary()
{
  TEST_BEGIN("TC-03 OK/WARNING boundary at 85C");

  ASSERT_TRUE(
      TemperatureClassifier::classify(84.9f) == TempStatus::Ok,
      "84.9C should be OK");

  ASSERT_TRUE(
      TemperatureClassifier::classify(85.0f) == TempStatus::Warning,
      "85.0C should be WARNING");

  TEST_PASS();
}

/* TC-04 */
static void test_warning_critical_boundary()
{
  TEST_BEGIN("TC-04 WARNING/CRITICAL boundary at 105C");

  ASSERT_TRUE(
      TemperatureClassifier::classify(104.9f) == TempStatus::Warning,
      "104.9C should be WARNING");

  ASSERT_TRUE(
      TemperatureClassifier::classify(105.0f) == TempStatus::Critical,
      "105.0C should be CRITICAL");

  TEST_PASS();
}

/* TC-05 */
static void test_critical_low_boundary()
{
  TEST_BEGIN("TC-05 CRITICAL low boundary at 5C");

  ASSERT_TRUE(
      TemperatureClassifier::classify(5.0f) == TempStatus::Ok,
      "5.0C should be OK");

  ASSERT_TRUE(
      TemperatureClassifier::classify(4.9f) == TempStatus::Critical,
      "4.9C should be CRITICAL");

  TEST_PASS();
}

/* TC-06 */
static void test_single_led_active()
{
  TEST_BEGIN("TC-06 exactly one LED lit per status");

  MockGpio gpio;
  gpio.init();

  LedController controller(gpio);

  float samples[] = {
      20.0f,
      90.0f,
      110.0f,
      2.0f
  };

  for (float sample : samples)
  {
    controller.update(
        TemperatureClassifier::classify(sample));

    int onCount =
        static_cast<int>(gpio.getLed(LedId::Green)) +
        static_cast<int>(gpio.getLed(LedId::Yellow)) +
        static_cast<int>(gpio.getLed(LedId::Red));

    ASSERT_EQ(onCount, 1,
              "exactly one LED must be ON");
  }

  TEST_PASS();
}

/* TC-07 */
static void test_eeprom_selects_revision()
{
  TEST_BEGIN("TC-07 EEPROM revision selects sensor resolution");

  MockEeprom eeprom;

  eeprom.setMockConfig(
      HwRevision::RevB,
      "ABC1234");

  HwConfig cfg;

  ASSERT_TRUE(
      eeprom.readConfig(cfg),
      "EEPROM read should succeed");

  ASSERT_TRUE(
      cfg.revision == HwRevision::RevB,
      "revision should be Rev-B");

  MockAdc adc;
  adc.init();

  TemperatureSensor sensor(
      adc,
      makeSensorResolutionStrategy(cfg.revision));

  float t = sensor.convert(100);

  ASSERT_TRUE(
      t > 9.99f && t < 10.01f,
      "Rev-B scaling should apply (raw=100 -> 10.0C)");

  eeprom.setMockConfig(
      HwRevision::RevA,
      "ABC1234");

  ASSERT_TRUE(
      eeprom.readConfig(cfg),
      "EEPROM read should succeed");

  ASSERT_TRUE(
      cfg.revision == HwRevision::RevA,
      "revision should be Rev-A");

  TEST_PASS();
}

/* TC-08 */
static void test_serial_roundtrip()
{
  TEST_BEGIN("TC-08 serial number round-trip");

  MockEeprom eeprom;

  eeprom.setMockConfig(
      HwRevision::RevA,
      "ABC1234");

  HwConfig cfg;

  ASSERT_TRUE(
      eeprom.readConfig(cfg),
      "EEPROM read should succeed");

  ASSERT_TRUE(
      cfg.serialNumber == "ABC1234",
      "serial number should round-trip unmodified");

  TEST_PASS();
}

/*
 * TC-10:
 * onTimerTick() represents the ISR entry point in the PC demonstration.
 * The test verifies that it is present, callable and performs the expected
 * sample acquisition/latching behaviour without a real hardware timer.
 */
static void test_isr_callable()
{
  TEST_BEGIN(
      "TC-10 ISR entry point (onTimerTick) present and callable");

  MockAdc adc;
  adc.init();

  MockGpio gpio;
  gpio.init();

  MockEeprom eeprom;

  eeprom.setMockConfig(
      HwRevision::RevA,
      "ABC1234");

  HwConfig cfg;

  ASSERT_TRUE(
      eeprom.readConfig(cfg),
      "EEPROM configuration should be available");

  TemperatureSensor sensor(
      adc,
      makeSensorResolutionStrategy(cfg.revision));

  LedController led(gpio);

  TemperatureMonitor monitor(
      adc,
      sensor,
      led);

  adc.setMockValue(42);

  ASSERT_FALSE(
      monitor.isSampleReady(),
      "sample-ready flag should start clear");

  monitor.onTimerTick();

  ASSERT_TRUE(
      monitor.isSampleReady(),
      "sample-ready flag should be set after onTimerTick()");

  monitor.processPendingSample();

  ASSERT_TRUE(
      monitor.lastTemperatureC() > 41.9f &&
      monitor.lastTemperatureC() < 42.1f,
      "processed sample should reflect the latched raw value");

  /*
   * Free-function trampoline used by the real hardware timer vector.
   */
  TimerIsr_RegisterMonitor(&monitor);

  isr_timer_100us_handler();

  ASSERT_TRUE(
      monitor.isSampleReady(),
      "isr_timer_100us_handler() should also be callable directly");

  TEST_PASS();
}

/* TC-11 */
static void test_invalid_hardware_revision()
{
  TEST_BEGIN("TC-11 invalid hardware revision rejected");

  auto strategy =
      makeSensorResolutionStrategy(
          static_cast<HwRevision>(99));

  ASSERT_TRUE(
      strategy == nullptr,
      "invalid hardware revision should be rejected");

  TEST_PASS();
}

int main()
{
  std::cout
      << "=== C++ unit tests (temp-monitor / cpp) ===\n";

  test_revA_conversion();
  test_revB_conversion();
  test_ok_warning_boundary();
  test_warning_critical_boundary();
  test_critical_low_boundary();
  test_single_led_active();
  test_eeprom_selects_revision();
  test_serial_roundtrip();
  test_isr_callable();
  test_invalid_hardware_revision();

  return testSummaryAndExitCode();
}