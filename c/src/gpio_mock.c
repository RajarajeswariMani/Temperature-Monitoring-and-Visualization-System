#include "gpio.h"
#include <stdio.h>

static const char *led_name(led_id_t led) {
  switch (led) {
  case LED_GREEN:
    return "GREEN";
  case LED_YELLOW:
    return "YELLOW";
  case LED_RED:
    return "RED";
  default:
    return "?";
  }
}

static bool s_led_state[LED_COUNT];

void GPIO_Init(void) {
  for (int i = 0; i < LED_COUNT; i++) {
    s_led_state[i] = false;
  }
}

void GPIO_SetLed(led_id_t led, bool on) {
  if (led >= LED_COUNT) {
    return;
  }
  if (s_led_state[led] != on) {
    s_led_state[led] = on;
    printf("  [GPIO] LED %-6s -> %s\n", led_name(led), on ? "ON" : "off");
  }
}

bool GPIO_GetLed(led_id_t led) {
  if (led >= LED_COUNT) {
    return false;
  }
  return s_led_state[led];
}
