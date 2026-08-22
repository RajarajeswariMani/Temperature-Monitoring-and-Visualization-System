#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* --- HAL: ADC driver interface ---
 * On real hardware this interface would access the ADC peripheral.
 * For the PC demonstration, adc.c provides a mocked implementation
 * so the rest of the software stack is independent of the hardware. */

void ADC_Init(void);
uint16_t ADC_ReadRaw(void);

/* --- Demo/test only: not part of the production HAL API --- */
void ADC_Mock_SetValue(uint16_t raw_value);

#endif /* ADC_H */
