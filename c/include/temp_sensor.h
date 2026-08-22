#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "eeprom.h"
#include <stdbool.h>
#include <stdint.h>

/* Temperature sensor processing.
 * Converts raw ADC values into degrees Celsius according to the
 * configured hardware revision (Rev-A or Rev-B). */
typedef struct {
  hw_revision_t revision;
} temp_sensor_t;

bool TempSensor_Init(temp_sensor_t *sensor, hw_revision_t revision);
float TempSensor_ConvertRawToCelsius(const temp_sensor_t *sensor, uint16_t raw);
float TempSensor_SampleCelsius(temp_sensor_t *sensor);

#endif /* TEMP_SENSOR_H */
