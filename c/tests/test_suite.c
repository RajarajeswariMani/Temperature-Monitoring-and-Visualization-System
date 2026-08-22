/*
 * Unit tests for the C implementation, using the project's own
 * dependency-free test framework (include/c_test_framework.h).
 *
 * The tests exercise the real production interfaces and implementation
 * through mocked hardware backends. No target hardware or emulator is
 * required.
 *
 * The automated tests cover the applicable functional test cases:
 * TC-01 through TC-08, TC-10, TC-11 and TC-12.
 *
 * TC-09, TC-13, TC-14 and TC-15 are verified separately through code
 * inspection, build/execution verification, C/C++ behavioural comparison
 * and compiler-warning verification as defined in TEST_PLAN.md.
 */

#include "adc.h"
#include "c_test_framework.h"
#include "eeprom.h"
#include "gpio.h"
#include "isr.h"
#include "led_controller.h"
#include "temp_sensor.h"

TestContext g_test_context;

/* TC-01: Rev-A conversion, 1 degC/digit */
static void test_revA_conversion(void)
{
  TEST_BEGIN("TC-01 Rev-A raw->Celsius (x1)");

  temp_sensor_t sensor;

  ASSERT_TRUE(
      TempSensor_Init(&sensor, HW_REV_A),
      "Rev-A sensor initialization should succeed");

  ASSERT_INT_EQ(
      (int)TempSensor_ConvertRawToCelsius(&sensor, 10),
      10,
      "raw=10 should be 10.0C");

  ASSERT_INT_EQ(
      (int)TempSensor_ConvertRawToCelsius(&sensor, 85),
      85,
      "raw=85 should be 85.0C");

  TEST_PASS();
}

/* TC-02: Rev-B conversion, 0.1 degC/digit */
static void test_revB_conversion(void)
{
  TEST_BEGIN("TC-02 Rev-B raw->Celsius (x0.1)");

  temp_sensor_t sensor;

  ASSERT_TRUE(
      TempSensor_Init(&sensor, HW_REV_B),
      "Rev-B sensor initialization should succeed");

  float t1 =
      TempSensor_ConvertRawToCelsius(&sensor, 100);

  float t2 =
      TempSensor_ConvertRawToCelsius(&sensor, 850);

  ASSERT_TRUE(
      t1 > 9.99f && t1 < 10.01f,
      "raw=100 should be 10.0C");

  ASSERT_TRUE(
      t2 > 84.99f && t2 < 85.01f,
      "raw=850 should be 85.0C");

  TEST_PASS();
}

/* TC-03: OK/Warning boundary */
static void test_ok_warning_boundary(void)
{
  TEST_BEGIN("TC-03 OK/WARNING boundary at 85C");

  ASSERT_INT_EQ(
      LedController_Classify(84.9f),
      TEMP_STATUS_OK,
      "84.9C should be OK");

  ASSERT_INT_EQ(
      LedController_Classify(85.0f),
      TEMP_STATUS_WARNING,
      "85.0C should be WARNING");

  TEST_PASS();
}

/* TC-04: Warning/Critical-high boundary */
static void test_warning_critical_boundary(void)
{
  TEST_BEGIN("TC-04 WARNING/CRITICAL boundary at 105C");

  ASSERT_INT_EQ(
      LedController_Classify(104.9f),
      TEMP_STATUS_WARNING,
      "104.9C should be WARNING");

  ASSERT_INT_EQ(
      LedController_Classify(105.0f),
      TEMP_STATUS_CRITICAL,
      "105.0C should be CRITICAL");

  TEST_PASS();
}

/* TC-05: Critical-low boundary */
static void test_critical_low_boundary(void)
{
  TEST_BEGIN("TC-05 CRITICAL low boundary at 5C");

  ASSERT_INT_EQ(
      LedController_Classify(5.0f),
      TEMP_STATUS_OK,
      "5.0C should be OK");

  ASSERT_INT_EQ(
      LedController_Classify(4.9f),
      TEMP_STATUS_CRITICAL,
      "4.9C should be CRITICAL");

  TEST_PASS();
}

/* TC-06: Exactly one LED lit at a time, for each status */
static void test_single_led_active(void)
{
  TEST_BEGIN("TC-06 exactly one LED lit per status");

  GPIO_Init();

  float samples[] = {
      20.0f,
      90.0f,
      110.0f,
      2.0f
  };

  for (size_t i = 0;
       i < sizeof(samples) / sizeof(samples[0]);
       ++i)
  {
    LedController_Update(samples[i]);

    int on_count =
        (int)GPIO_GetLed(LED_GREEN) +
        (int)GPIO_GetLed(LED_YELLOW) +
        (int)GPIO_GetLed(LED_RED);

    ASSERT_INT_EQ(
        on_count,
        1,
        "exactly one LED must be ON");
  }

  TEST_PASS();
}

/* TC-07: EEPROM-selected revision drives sensor resolution */
static void test_eeprom_selects_revision(void)
{
  TEST_BEGIN(
      "TC-07 EEPROM revision selects sensor resolution");

  EEPROM_Mock_SetConfig(
      HW_REV_B,
      "ABC1234");

  hw_config_t cfg;

  ASSERT_TRUE(
      EEPROM_ReadConfig(&cfg),
      "EEPROM read should succeed");

  ASSERT_INT_EQ(
      cfg.hw_revision,
      HW_REV_B,
      "revision should be Rev-B");

  temp_sensor_t sensor;

  ASSERT_TRUE(
      TempSensor_Init(&sensor, cfg.hw_revision),
      "sensor initialization should succeed");

  float t =
      TempSensor_ConvertRawToCelsius(
          &sensor,
          100);

  ASSERT_TRUE(
      t > 9.99f && t < 10.01f,
      "Rev-B scaling should apply (raw=100 -> 10.0C)");

  EEPROM_Mock_SetConfig(
      HW_REV_A,
      "ABC1234");

  ASSERT_TRUE(
      EEPROM_ReadConfig(&cfg),
      "EEPROM read should succeed");

  ASSERT_INT_EQ(
      cfg.hw_revision,
      HW_REV_A,
      "revision should be Rev-A");

  TEST_PASS();
}

/* TC-08: Serial number round-trips through EEPROM mock unmodified */
static void test_serial_roundtrip(void)
{
  TEST_BEGIN("TC-08 serial number round-trip");

  EEPROM_Mock_SetConfig(
      HW_REV_A,
      "ABC1234");

  hw_config_t cfg;

  ASSERT_TRUE(
      EEPROM_ReadConfig(&cfg),
      "EEPROM read should succeed");

  ASSERT_STR_EQ(
      cfg.serial_number,
      "ABC1234",
      "serial number should round-trip unmodified");

  TEST_PASS();
}

/*
 * TC-10:
 * ISR entry point is defined, typed and callable without a real timer.
 */
static void test_isr_callable(void)
{
  TEST_BEGIN(
      "TC-10 ISR entry point present and callable");

  ADC_Mock_SetValue(42);

  ISR_ClearSampleReady();

  ASSERT_FALSE(
      ISR_IsSampleReady(),
      "sample-ready flag should start clear");

  ISR_Timer100us_Handler();

  ASSERT_TRUE(
      ISR_IsSampleReady(),
      "sample-ready flag should be set after ISR call");

  ASSERT_INT_EQ(
      ISR_GetLatchedRaw(),
      42,
      "ISR should latch the raw ADC value");

  ISR_ClearSampleReady();

  TEST_PASS();
}

/*
 * TC-15:
 * Clean compilation with -Wall -Wextra -Wpedantic is a build-level
 * verification performed by the Makefile; it is not repeated here.
 */

/* TC-11: Invalid hardware revision should be rejected */
static void test_invalid_hardware_revision(void)
{
  TEST_BEGIN(
      "TC-11 invalid hardware revision rejected");

  temp_sensor_t sensor;

  ASSERT_FALSE(
      TempSensor_Init(
          &sensor,
          (hw_revision_t)99),
      "invalid hardware revision should be rejected");

  TEST_PASS();
}

/* TC-12: NULL sensor pointer should be rejected safely */
static void test_null_sensor_pointer(void)
{
  TEST_BEGIN(
      "TC-12 NULL sensor pointer rejected");

  ASSERT_FALSE(
      TempSensor_Init(
          NULL,
          HW_REV_A),
      "NULL sensor pointer should be rejected");

  TEST_PASS();
}

int main(void)
{
  TEST_INIT();

  printf(
      "=== C unit tests (temp-monitor / c) ===\n");

  ADC_Init();
  GPIO_Init();
  EEPROM_Init();

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
  test_null_sensor_pointer();

  TEST_SUMMARY();

  return TEST_EXIT_CODE();
}