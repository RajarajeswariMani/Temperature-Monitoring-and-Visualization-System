#include "eeprom.h"
#include <string.h>

/* Mocked EEPROM. Defaults to Rev-A hardware with a placeholder serial
 * number; the demo can override this via EEPROM_Mock_SetConfig() to
 * exercise the Rev-B code path. */
static hw_config_t s_mock_cfg = {.hw_revision = HW_REV_A,
                                 .serial_number = "ABC1234"};

void EEPROM_Init(void) { /* no-op for the mock */ }

bool EEPROM_ReadConfig(hw_config_t *out_cfg) {
  if (out_cfg == NULL) {
    return false;
  }
  *out_cfg = s_mock_cfg;
  return true;
}

void EEPROM_Mock_SetConfig(hw_revision_t revision, const char *serial) {
  s_mock_cfg.hw_revision = revision;

  if (serial == NULL) {
    s_mock_cfg.serial_number[0] = '\0';
    return;
  }

  strncpy(s_mock_cfg.serial_number, serial, EEPROM_SERIAL_LEN - 1);
  s_mock_cfg.serial_number[EEPROM_SERIAL_LEN - 1] = '\0';
}
