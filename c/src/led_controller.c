#include "led_controller.h"
#include "config.h"
#include "gpio.h"

temp_status_t LedController_Classify(float temp_c) {
  if (temp_c >= TEMP_THRESHOLD_CRITICAL_HI_C ||
      temp_c < TEMP_THRESHOLD_CRITICAL_LO_C) {
    return TEMP_STATUS_CRITICAL;
  }
  if (temp_c >= TEMP_THRESHOLD_WARNING_C) {
    return TEMP_STATUS_WARNING;
  }
  return TEMP_STATUS_OK;
}

void LedController_Update(float temp_c) {
  temp_status_t status = LedController_Classify(temp_c);

  /* Exactly one LED is lit at a time. */
  GPIO_SetLed(LED_GREEN, status == TEMP_STATUS_OK);
  GPIO_SetLed(LED_YELLOW, status == TEMP_STATUS_WARNING);
  GPIO_SetLed(LED_RED, status == TEMP_STATUS_CRITICAL);
}
