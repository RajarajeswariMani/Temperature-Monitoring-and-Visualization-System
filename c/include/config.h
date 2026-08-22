#ifndef CONFIG_H
#define CONFIG_H

/* Sampling period for the temperature ADC, driven by a hardware timer
 * interrupt. 100us is required to keep jitter low -> the ISR itself
 * only latches data (see isr.c); no floating point / heavy work in
 * interrupt context. */
#define TEMP_SAMPLE_PERIOD_US 100u

/* Temperature classification thresholds, in degrees Celsius. */
#define TEMP_THRESHOLD_WARNING_C 85.0f
#define TEMP_THRESHOLD_CRITICAL_HI_C 105.0f
#define TEMP_THRESHOLD_CRITICAL_LO_C 5.0f

/* EEPROM layout */
#define EEPROM_SERIAL_LEN 21u /* 20 characters + NUL terminator */

#endif /* CONFIG_H */
