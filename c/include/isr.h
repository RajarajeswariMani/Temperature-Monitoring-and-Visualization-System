#ifndef ISR_H
#define ISR_H

#include <stdbool.h>
#include <stdint.h>

/* Fired by a hardware timer peripheral every TEMP_SAMPLE_PERIOD_US
 * (100us). Kept minimal on purpose to guarantee low, deterministic
 * jitter: it only latches the raw ADC reading and raises a flag.
 * All conversion / classification / LED work is deferred to the main
 * application loop (isr_deferred processing pattern).
 *
 * NOTE: on the PC demonstration build this is never attached to a
 * real interrupt vector; main.c calls it directly to simulate ticks. */
void ISR_Timer100us_Handler(void);

bool ISR_IsSampleReady(void);
uint16_t ISR_GetLatchedRaw(void);
void ISR_ClearSampleReady(void);

#endif /* ISR_H */
