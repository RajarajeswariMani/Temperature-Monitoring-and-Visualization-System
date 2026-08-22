#ifndef EEPROM_H
#define EEPROM_H

#include "config.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  HW_REV_A = 0, /* 1 degC / digit sensor  */
  HW_REV_B = 1  /* 0.1 degC / digit sensor */
} hw_revision_t;

typedef struct {
  hw_revision_t hw_revision;
  char serial_number[EEPROM_SERIAL_LEN]; /* placeholder, e.g. "ABC1234" */
} hw_config_t;

void EEPROM_Init(void);
bool EEPROM_ReadConfig(hw_config_t *out_cfg);

/* --- Demo/test only --- */
void EEPROM_Mock_SetConfig(hw_revision_t revision, const char *serial);

#endif /* EEPROM_H */
