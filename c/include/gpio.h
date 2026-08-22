#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

typedef enum { LED_GREEN = 0, LED_YELLOW, LED_RED, LED_COUNT } led_id_t;

void GPIO_Init(void);
void GPIO_SetLed(led_id_t led, bool on);
bool GPIO_GetLed(led_id_t led);

#endif /* GPIO_H */
