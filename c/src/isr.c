#include "isr.h"
#include "adc.h"

static volatile uint16_t s_latched_raw = 0u;
static volatile bool s_sample_ready = false;

void ISR_Timer100us_Handler(void) {
  /* Minimal-latency work only: latch + flag. Do NOT add float math,
   * printing, or anything non-deterministic here - that is what
   * causes jitter on the 100us period. */
  s_latched_raw = ADC_ReadRaw();
  s_sample_ready = true;
}

bool ISR_IsSampleReady(void) { return s_sample_ready; }

uint16_t ISR_GetLatchedRaw(void) { return s_latched_raw; }

void ISR_ClearSampleReady(void) { s_sample_ready = false; }
