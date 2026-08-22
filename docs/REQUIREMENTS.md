# Requirements

This document defines the system, hardware, software, and error-handling requirements for the Temperature Monitoring System.

The requirements are derived from the original task specification and are traced to the architecture, implementation, and verification activities documented in `TEST_PLAN.md`.

---

## 1. System Requirements

| ID    | Requirement                                                                                                                                                                                                                  | Satisfied by                                                     | Verification        |
| ----- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------- |
| SYS-1 | The system shall monitor temperature and visualize the temperature condition using three LEDs: Green, Yellow, and Red.                                                                                                       | Temperature monitoring flow, LED controller, and GPIO interface. | TC-06, TC-13, TC-14 |
| SYS-2 | The architecture shall be documented using architecture diagrams in `.pdf`, `.png`, or `.svg` format.                                                                                                                        | `docs/diagrams.md` and `docs/diagrams/`.                         | TC-16               |
| SYS-3 | The software shall be demonstrable on a PC using mocked hardware interfaces. No target hardware or hardware emulator is required for the demonstration.                                                                      | C mock hardware interfaces and C++ mock classes.                 | TC-13               |
| SYS-4 | Two implementations shall be provided: a C implementation and a C++ implementation using object-oriented programming principles.                                                                                             | `c/` and `cpp/` implementations.                                 | TC-13, TC-14, TC-15 |
| SYS-5 | The complete project, including source code, tests, documentation, and architecture diagrams, shall be uploaded to a public GitHub repository. The repository link shall be provided at least 24 hours before the interview. | Public GitHub repository.                                        | TC-17               |

---

## 2. Hardware Requirements

| ID   | Requirement                                                                                                                                                                              | Satisfied by                                                                                       | Verification        |
| ---- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ------------------- |
| HW-1 | A temperature sensor shall be connected to the microcontroller through an ADC channel.                                                                                                   | C: `adc.h`; C++: `IAdc` and `TemperatureSensor`; mocked ADC for PC demonstration.                  | TC-01, TC-02        |
| HW-2 | Three LEDs shall be connected through GPIOs to visualize temperature status: Green, Yellow, and Red.                                                                                     | C: `gpio.h`; C++: `IGpio`, `MockGpio`, and `LedController`.                                        | TC-06               |
| HW-3 | An EEPROM shall provide the hardware revision and hardware serial number configuration. For the PC demonstration, the physical EEPROM shall be represented by a mocked EEPROM interface. | C: `eeprom.h`; C++: `IEeprom` and `MockEeprom`.                                                    | TC-07, TC-08        |
| HW-4 | The system shall support two hardware revisions: Rev-A with 1°C per digit and Rev-B with 0.1°C per digit. Only the configured sensor type shall be active at runtime.                    | C: `TempSensor_ConvertRawToCelsius()`; C++: `ISensorResolutionStrategy`.                           | TC-01, TC-02, TC-07 |
| HW-5 | A timer peripheral shall generate a periodic interrupt every 100 µs to trigger temperature sampling.                                                                                     | C: `ISR_Timer100us_Handler()`; C++: `TemperatureMonitor::onTimerTick()` and timer ISR entry point. | TC-09, TC-10        |

---

## 3. Temperature Conversion Requirements

| ID     | Requirement                                                                                                                                    | Verification |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------------------- | ------------ |
| TEMP-1 | For Rev-A, an ADC value shall represent temperature directly in degrees Celsius. For example, raw value `10` shall represent `10°C`.           | TC-01        |
| TEMP-2 | For Rev-B, an ADC value shall represent temperature with a resolution of 0.1°C per digit. For example, raw value `100` shall represent `10°C`. | TC-02        |
| TEMP-3 | The sensor conversion shall use the hardware revision obtained from the EEPROM configuration.                                                  | TC-07        |
| TEMP-4 | An invalid hardware revision shall not be accepted during sensor initialization.                                                               | TC-11        |

---

## 4. Temperature Classification and LED Requirements

| ID    | Requirement                                                                                   | Verification        |
| ----- | --------------------------------------------------------------------------------------------- | ------------------- |
| LED-1 | Green LED shall indicate normal operation when `5°C ≤ T < 85°C`.                              | TC-03, TC-06        |
| LED-2 | Yellow LED shall indicate the warning range when `85°C ≤ T < 105°C`.                          | TC-03, TC-04, TC-06 |
| LED-3 | Red LED shall indicate the critical range when `T ≥ 105°C` or `T < 5°C`.                      | TC-04, TC-05, TC-06 |
| LED-4 | Exactly one status LED shall be active for every valid temperature classification.            | TC-06               |
| LED-5 | The temperature boundaries shall be handled explicitly, including `5°C`, `85°C`, and `105°C`. | TC-03, TC-04, TC-05 |

---

## 5. Software Requirements

| ID   | Requirement                                                                                                                                                                                                  | Satisfied by                                                             | Verification        |
| ---- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ | ------------------- |
| SW-1 | The target software shall sample temperature every 100 µs with very low jitter.                                                                                                                              | Timer ISR and temperature sampling architecture.                         | TC-09, TC-10        |
| SW-2 | The 100 µs ISR shall perform only minimal deterministic work: ADC read, raw-value latch, and sample-ready indication. Temperature conversion, classification, and LED control shall execute outside the ISR. | C ISR and C++ `TemperatureMonitor::onTimerTick()`.                       | TC-09               |
| SW-3 | The software shall convert the raw ADC value into degrees Celsius according to the configured hardware revision.                                                                                             | C: `TempSensor_ConvertRawToCelsius()`; C++: resolution strategy classes. | TC-01, TC-02        |
| SW-4 | The software shall read the hardware revision and serial number from EEPROM during initialization.                                                                                                           | C: `EEPROM_ReadConfig()`; C++: `IEeprom::readConfig()`.                  | TC-07, TC-08        |
| SW-5 | ISR function(s) shall be defined and implemented even though the PC demonstration does not require a real hardware interrupt.                                                                                | C: `ISR_Timer100us_Handler()`; C++: `isr_timer_100us_handler()`.         | TC-10               |
| SW-6 | The C implementation shall follow a bare-metal/procedural embedded software structure while allowing mocked hardware interfaces for PC execution.                                                            | `c/` implementation and mock HAL.                                        | TC-13, TC-15        |
| SW-7 | The C++ implementation shall provide equivalent functional behavior using OOP principles, including interfaces, encapsulation, dependency injection, and the Strategy pattern.                               | `cpp/` implementation.                                                   | TC-13, TC-14, TC-15 |
| SW-8 | ADC, GPIO, EEPROM, and other hardware-dependent interfaces shall be abstracted so that the application can be demonstrated and tested on a PC without physical hardware.                                     | C HAL/mocks and C++ interfaces/mocks.                                    | TC-13               |

---

## 6. Error Handling Requirements

| ID    | Requirement                                                                                | Satisfied by                                                         | Verification |
| ----- | ------------------------------------------------------------------------------------------ | -------------------------------------------------------------------- | ------------ |
| ERR-1 | An invalid hardware revision shall be detected and rejected during sensor initialization.  | C: `TempSensor_Init()` validation; C++ strategy creation/validation. | TC-11        |
| ERR-2 | A NULL sensor pointer shall be handled safely by the C sensor initialization function.     | C: `TempSensor_Init()` NULL validation.                              | TC-12        |
| ERR-3 | Unsupported sensor configuration shall not silently select an incorrect sensor resolution. | C and C++ hardware-revision validation.                              | TC-11        |

---

## 7. ISR and Timing Requirements

The system requires a 100 µs periodic sampling interval with very low jitter.

The PC demonstration cannot prove the real-time characteristics of a bare-metal MCU timer. Therefore, the implementation separates the real-time ISR responsibilities from deferred application processing.

### ISR responsibilities

The ISR shall:

1. Be entered from the 100 µs timer event.
2. Read the ADC value.
3. Latch the raw ADC value.
4. Set a sample-ready indication.
5. Return as quickly as possible.

The ISR shall **not** perform:

* Temperature conversion
* Temperature classification
* LED control
* EEPROM access
* Long-running processing

### Deferred application responsibilities

The main/application context shall:

1. Detect the sample-ready indication.
2. Convert the latched ADC value to temperature.
3. Classify the temperature.
4. Update the appropriate LED.
5. Continue processing the application loop.

Actual MCU timing, timer accuracy, jitter, ADC sampling timing, and ISR execution time shall be verified when the software is ported to the target hardware.

---

## 8. PC Demonstration Requirements

The PC demonstration shall provide a hardware-independent environment for demonstrating the application logic.

The demonstration shall:

* Use mocked ADC input.
* Use mocked GPIO outputs.
* Use a mocked EEPROM configuration.
* Allow Rev-A and Rev-B configurations to be demonstrated.
* Demonstrate temperature classification.
* Demonstrate Green, Yellow, and Red LED behavior.
* Provide C and C++ implementations.
* Not require physical target hardware.
* Not require an MCU emulator.

The simulated timer shall invoke the implemented ISR entry point to demonstrate the intended runtime flow.

---

## 9. Architecture Requirements

The architecture documentation shall describe the system at multiple abstraction levels.

The architecture documentation shall include:

* Overall system architecture
* System static architecture
* Hardware static architecture
* C software static architecture
* C++ software static architecture
* C software dynamic architecture
* C++ software dynamic architecture

The static views shall describe system structure, components, interfaces, and dependencies.

The dynamic views shall describe runtime behavior, including the separation between:

**100 µs timer/ISR context → deferred application processing**

Architecture documentation is maintained in:

`docs/diagrams.md`

Pre-rendered diagrams are maintained in:

`docs/diagrams/`

---

## 10. Traceability

The project maintains bidirectional traceability:

**Requirement → Architecture → Implementation → Test**

and:

**Test → Requirement**

The detailed verification strategy is maintained in `TEST_PLAN.md`.

The architecture documentation provides traceability between:

**Requirements → Architecture → Components → Runtime Behavior**

The implementation is provided in two forms:

* C bare-metal/procedural implementation
* C++ object-oriented implementation

---

## 11. Verification Strategy

Verification shall use a combination of:

* Automated unit tests
* Build verification
* PC demonstration
* Code inspection
* Architecture review
* C/C++ behavioral comparison
* Repository verification
* Target-hardware verification where required

### Automated Unit Test Coverage

The automated unit-test suite shall cover the applicable functional cases:

* TC-01 — Rev-A ADC conversion
* TC-02 — Rev-B ADC conversion
* TC-03 — 85°C boundary
* TC-04 — 105°C boundary
* TC-05 — 5°C critical-low boundary
* TC-06 — Exactly one LED active
* TC-07 — EEPROM hardware revision configuration
* TC-08 — EEPROM serial number
* TC-10 — ISR entry point and sampling behavior
* TC-11 — Invalid hardware revision
* TC-12 — NULL sensor pointer

### Non-Automated Verification

The following verification activities require inspection, execution, comparison, or repository-level checks:

* **TC-09** — ISR minimal-work verification by code inspection
* **TC-13** — C and C++ PC demonstration build and execution
* **TC-14** — C and C++ behavioral comparison
* **TC-15** — Clean compilation with required compiler warnings enabled
* **TC-16** — Architecture documentation review
* **TC-17** — Public repository and clean-clone verification

---

## 12. Target Hardware Verification

The PC demonstration cannot verify actual MCU real-time performance.

When ported to target hardware, the following shall be verified:

* 100 µs timer period
* Timer configuration
* Timer interrupt jitter
* ADC sampling timing
* ISR execution time
* ISR execution-time margin relative to the 100 µs period

The PC implementation therefore verifies the software architecture and functional behavior, while the target platform is responsible for final real-time timing verification.

---

## 13. Requirement-to-Test Summary

| Requirement | Verification        |
| ----------- | ------------------- |
| SYS-1       | TC-06, TC-13, TC-14 |
| SYS-2       | TC-16               |
| SYS-3       | TC-13               |
| SYS-4       | TC-13, TC-14, TC-15 |
| SYS-5       | TC-17               |
| HW-1        | TC-01, TC-02        |
| HW-2        | TC-06               |
| HW-3        | TC-07, TC-08        |
| HW-4        | TC-01, TC-02, TC-07 |
| HW-5        | TC-09, TC-10        |
| TEMP-1      | TC-01               |
| TEMP-2      | TC-02               |
| TEMP-3      | TC-07               |
| TEMP-4      | TC-11               |
| LED-1       | TC-03, TC-06        |
| LED-2       | TC-03, TC-04, TC-06 |
| LED-3       | TC-04, TC-05, TC-06 |
| LED-4       | TC-06               |
| LED-5       | TC-03, TC-04, TC-05 |
| SW-1        | TC-09, TC-10        |
| SW-2        | TC-09               |
| SW-3        | TC-01, TC-02        |
| SW-4        | TC-07, TC-08        |
| SW-5        | TC-10               |
| SW-6        | TC-13, TC-15        |
| SW-7        | TC-13, TC-14, TC-15 |
| SW-8        | TC-13               |
| ERR-1       | TC-11               |
| ERR-2       | TC-12               |
| ERR-3       | TC-11               |

---

## 14. Coverage Statement

The requirements, architecture, implementation, and verification activities are maintained with bidirectional traceability:

**Requirement → Architecture → Implementation → Test**

and:

**Test → Requirement**

The PC demonstration verifies functional behavior using mocked hardware.

The C implementation provides the procedural/bare-metal-oriented version required for embedded deployment.

The C++ implementation provides equivalent functionality using object-oriented design principles.

Actual MCU-specific real-time characteristics, including the 100 µs timer accuracy, interrupt jitter, ADC sampling timing, and ISR execution-time constraints, remain target-hardware verification activities.

---

## 15. Task Compliance Summary

| Original Task Requirement            | Status                                  |
| ------------------------------------ | --------------------------------------- |
| Temperature sensor connected via ADC | Covered by HW-1                         |
| Sampling every 100 µs                | Covered by HW-5 / SW-1                  |
| Very low jitter                      | Covered by SW-1 and target verification |
| Green for normal temperature         | Covered by LED-1                        |
| Yellow for warning temperature       | Covered by LED-2                        |
| Red for critical temperature         | Covered by LED-3                        |
| Rev-A sensor resolution              | Covered by TEMP-1                       |
| Rev-B sensor resolution              | Covered by TEMP-2                       |
| EEPROM hardware revision             | Covered by HW-3 / SW-4                  |
| EEPROM serial number                 | Covered by HW-3 / SW-4                  |
| PC demonstration                     | Covered by SYS-3                        |
| Mocked hardware                      | Covered by SW-8                         |
| C implementation                     | Covered by SYS-4 / SW-6                 |
| ISR implementation                   | Covered by SW-5                         |
| C++ OOP implementation               | Covered by SYS-4 / SW-7                 |
| Architecture documentation           | Covered by SYS-2                        |
| Public GitHub repository             | Covered by SYS-5                        |

---

**End of Requirements**
