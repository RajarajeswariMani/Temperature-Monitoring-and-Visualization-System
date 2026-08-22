#include "adc.h"

/* Mocked ADC: on the real target this would read a peripheral
 * register. Here we just return whatever value the demo/test code
 * has poked in, defaulting to 0. */
static uint16_t s_mock_raw = 0u;

void ADC_Init(void) { s_mock_raw = 0u; }

uint16_t ADC_ReadRaw(void) { return s_mock_raw; }

void ADC_Mock_SetValue(uint16_t raw_value) { s_mock_raw = raw_value; }
