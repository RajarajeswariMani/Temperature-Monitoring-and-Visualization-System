/* PC demonstration harness for the temperature monitoring firmware.
 *
 * This is NOT the target firmware entry point - on the real hardware
 * the timer peripheral would fire ISR_Timer100us_Handler() every
 * 100us and there would be no main() loop like this. This program
 * exists purely to exercise the architecture end-to-end using the
 * mocked HAL (adc_mock.c / gpio_mock.c / eeprom_mock.c) so it can run
 * on a PC with no target hardware attached. */

#include "adc.h"
#include "config.h"
#include "eeprom.h"
#include "gpio.h"
#include "isr.h"
#include "led_controller.h"
#include "temp_sensor.h"
#include <stdio.h>

typedef struct {
  const char *label;
  uint16_t raw_rev_a; /* raw digit to feed for Rev-A demo run   */
  uint16_t raw_rev_b; /* raw digit to feed for Rev-B demo run   */
} demo_point_t;

static void run_demo_for_revision(hw_revision_t revision, const char *serial) {
  static const demo_point_t points[] = {
      {"OK (low-normal)", 20, 200},    {"OK (high-normal)", 60, 600},
      {"WARNING (>=85C)", 90, 900},    {"CRITICAL (>=105C)", 110, 1100},
      {"CRITICAL (<5C, cold)", 2, 20}, {"back to OK", 25, 250},
  };
  const size_t n_points = sizeof(points) / sizeof(points[0]);

  EEPROM_Mock_SetConfig(revision, serial);

  hw_config_t cfg;
  if (!EEPROM_ReadConfig(&cfg)) {
    printf("Failed to read EEPROM configuration!\n");
    return;
  }

  temp_sensor_t sensor;

  if (!TempSensor_Init(&sensor, cfg.hw_revision)) {
    printf("Invalid hardware revision!\n");
    return;
  }

  printf("\n=== Hardware Revision: %s | Serial: %s ===\n",
         cfg.hw_revision == HW_REV_A ? "Rev-A (1 degC/digit)"
                                     : "Rev-B (0.1 degC/digit)",
         cfg.serial_number);

  for (size_t i = 0; i < n_points; i++) {
    uint16_t raw = (cfg.hw_revision == HW_REV_A) ? points[i].raw_rev_a
                                                 : points[i].raw_rev_b;

    /* Simulate the sensor producing a new raw value, then simulate
     * one 100us timer tick firing the ISR. */
    ADC_Mock_SetValue(raw);
    ISR_Timer100us_Handler();

    if (ISR_IsSampleReady()) {
      uint16_t latched = ISR_GetLatchedRaw();
      ISR_ClearSampleReady();

      float temp_c = TempSensor_ConvertRawToCelsius(&sensor, latched);
      temp_status_t status = LedController_Classify(temp_c);
      LedController_Update(temp_c);

      const char *status_str = (status == TEMP_STATUS_OK)        ? "OK"
                               : (status == TEMP_STATUS_WARNING) ? "WARNING"
                                                                 : "CRITICAL";

      printf("[%-24s] raw=%-5u temp=%6.1fC status=%s\n", points[i].label,
             latched, (double)temp_c, status_str);
    }
  }
}

int main(void) {
  printf("Temperature Monitoring & Visualization - C demo\n");
  printf("Sample period target: %u us (see isr.c for jitter discussion)\n",
         TEMP_SAMPLE_PERIOD_US);

  ADC_Init();
  GPIO_Init();
  EEPROM_Init();

  run_demo_for_revision(HW_REV_A, "ABC1234");
  run_demo_for_revision(HW_REV_B, "XYZ9876");

  return 0;
}
