#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

typedef enum {
  TEMP_STATUS_OK = 0,
  TEMP_STATUS_WARNING,
  TEMP_STATUS_CRITICAL
} temp_status_t;

/* Classifies a temperature reading against the thresholds in config.h. */
temp_status_t LedController_Classify(float temp_c);

/* Drives exactly one of the three LEDs according to the classification. */
void LedController_Update(float temp_c);

#endif /* LED_CONTROLLER_H */
