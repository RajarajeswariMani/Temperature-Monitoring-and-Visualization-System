#include "temp_sensor.h"
#include "adc.h"
#include <stddef.h>

bool TempSensor_Init(temp_sensor_t *sensor, hw_revision_t revision) {
  if (sensor == NULL) {
    return false;
  }

  if (revision != HW_REV_A && revision != HW_REV_B) {
    return false;
  }

  sensor->revision = revision;
  return true;
}
float TempSensor_ConvertRawToCelsius(const temp_sensor_t *sensor,
                                     uint16_t raw) {
  if (sensor == NULL) {
    return 0.0f;
  }

  switch (sensor->revision) {
    case HW_REV_A:
      return (float)raw;

    case HW_REV_B:
      return (float)raw * 0.1f;

    default:
      return 0.0f;
  }
}

float TempSensor_SampleCelsius(temp_sensor_t *sensor) {
  uint16_t raw = ADC_ReadRaw();
  return TempSensor_ConvertRawToCelsius(sensor, raw);
}
