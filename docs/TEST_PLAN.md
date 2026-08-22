# Test Plan

## 1. Purpose

This test plan defines the verification strategy for the Temperature Monitoring System and verifies the requirements defined in `REQUIREMENTS.md`.

The verification methods are:

* Automated unit tests
* Build verification
* PC demonstration
* Code inspection
* Manual architecture review
* C/C++ behavioral comparison
* Repository verification
* Target-hardware verification where applicable

The test plan covers verification activities from **TC-01 through TC-17**.

---

## 2. Test Environment

### C Implementation

* C11
* GCC
* `-Wall -Wextra -Wpedantic`
* PC / Linux / WSL
* Mocked hardware interfaces
* Make

### C++ Implementation

* C++17
* G++
* `-Wall -Wextra -Wpedantic`
* PC / Linux / WSL
* Mocked hardware interfaces
* Make

### Hardware

No physical target hardware or emulator is required for the PC demonstration.

The 100 µs timer interrupt is represented by invoking the implemented ISR entry point from the PC demonstration or test environment.

Actual MCU timer accuracy, interrupt jitter, ADC sampling timing, and ISR execution time shall be verified separately when the software is ported to target hardware.

---

# 3. Test Case Matrix

| TC    | Requirement(s)                                                                      | Description                                                                                                                                                                    | Method                              | Result  |
| ----- | ----------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------- | ------- |
| TC-01 | TEMP-1, SW-3                                                                        | Verify Rev-A conversion: raw `10` → `10.0°C` and raw `85` → `85.0°C`.                                                                                                          | Automated unit test                 | PASS    |
| TC-02 | TEMP-2, SW-3                                                                        | Verify Rev-B conversion: raw `100` → `10.0°C` and raw `850` → `85.0°C`.                                                                                                        | Automated unit test                 | PASS    |
| TC-03 | LED-1, LED-2, LED-5                                                                 | Verify the 85°C boundary: `84.9°C` → Green/OK and `85.0°C` → Yellow/Warning.                                                                                                   | Automated unit test                 | PASS    |
| TC-04 | LED-2, LED-3, LED-5                                                                 | Verify the 105°C boundary: `104.9°C` → Yellow/Warning and `105.0°C` → Red/Critical.                                                                                            | Automated unit test                 | PASS    |
| TC-05 | LED-1, LED-3, LED-5                                                                 | Verify the lower boundary: `5.0°C` → Green/OK and `4.9°C` → Red/Critical.                                                                                                      | Automated unit test                 | PASS    |
| TC-06 | SYS-1, HW-2, LED-4                                                                  | Verify exactly one LED is active for OK, Warning, and Critical conditions.                                                                                                     | Automated unit test using GPIO mock | PASS    |
| TC-07 | HW-3, HW-4, TEMP-3, SW-4                                                            | Verify EEPROM hardware revision selects the correct sensor resolution.                                                                                                         | Automated unit test                 | PASS    |
| TC-08 | HW-3, SW-4                                                                          | Verify EEPROM serial number is read and preserved correctly.                                                                                                                   | Automated unit test                 | PASS    |
| TC-09 | SW-1, SW-2, HW-5                                                                    | Verify the ISR performs only minimal deterministic work: ADC read, raw-value latch, and sample-ready indication.                                                               | Code inspection                     | PASS    |
| TC-10 | SW-1, SW-5, HW-5                                                                    | Verify the 100 µs ISR entry point exists, has the expected interface/type, and can be invoked without real hardware.                                                           | Automated unit test                 | PASS    |
| TC-11 | TEMP-4, ERR-1, ERR-3                                                                | Verify an invalid hardware revision is detected and rejected safely during sensor initialization.                                                                              | Automated unit test                 | PASS    |
| TC-12 | ERR-2                                                                               | Verify a NULL sensor pointer is handled safely by the C sensor initialization function.                                                                                        | Automated unit test                 | PASS    |
| TC-13 | SYS-1, SYS-3, SYS-4, SW-6, SW-7, SW-8                                               | Verify both C and C++ PC demonstrations build and run using mocked hardware and demonstrate temperature, classification, and LED behavior.                                     | Build + execution                   | PASS    |
| TC-14 | SYS-1, SYS-4, SW-2, SW-3, SW-7                                                      | Verify C and C++ implementations produce equivalent temperature, status, and LED behavior for equivalent input sequences.                                                      | Manual comparison                   | PASS    |
| TC-15 | SYS-4, SW-6, SW-7                                                                   | Verify both C and C++ projects compile cleanly using `-Wall -Wextra -Wpedantic`.                                                                                               | Build test                          | PASS    |
| TC-16 | SYS-2, HW-1, HW-2, HW-3, HW-4, HW-5, SW-1, SW-2, SW-3, SW-4, SW-5, SW-6, SW-7, SW-8 | Verify the overall, static, and dynamic architecture diagrams are consistent with the implemented hardware and software design.                                                | Manual architecture review          | PASS    |
| TC-17 | SYS-5                                                                               | Verify the public GitHub repository contains source code, tests, requirements, test plan, documentation, and diagrams, and that both projects can be built from a clean clone. | Repository/process check            | PASS |

---

# 4. Detailed Test Cases

## TC-01 — Rev-A Temperature Conversion

**Requirements:** `TEMP-1`, `SW-3`

### Objective

Verify that Rev-A uses a resolution of 1°C per digit.

### Test Inputs

```text
Hardware revision = Rev-A
Raw ADC = 10
Raw ADC = 85
```

### Expected Results

```text
Raw ADC = 10  → 10.0°C
Raw ADC = 85  → 85.0°C
```

### Verification

The automated C and C++ unit tests shall verify the conversion result.

### Result

**PASS**

---

## TC-02 — Rev-B Temperature Conversion

**Requirements:** `TEMP-2`, `SW-3`

### Objective

Verify that Rev-B uses a resolution of 0.1°C per digit.

### Test Inputs

```text
Hardware revision = Rev-B
Raw ADC = 100
Raw ADC = 850
```

### Expected Results

```text
Raw ADC = 100 → 10.0°C
Raw ADC = 850 → 85.0°C
```

### Verification

The automated C and C++ unit tests shall verify the conversion result.

### Result

**PASS**

---

## TC-03 — OK/Warning Boundary

**Requirements:** `LED-1`, `LED-2`, `LED-5`

### Objective

Verify the transition between normal and warning temperature ranges.

### Test Inputs

```text
Temperature = 84.9°C
Temperature = 85.0°C
```

### Expected Results

```text
84.9°C → OK → Green LED
85.0°C → Warning → Yellow LED
```

### Result

**PASS**

---

## TC-04 — Warning/Critical Boundary

**Requirements:** `LED-2`, `LED-3`, `LED-5`

### Objective

Verify the transition between warning and critical temperature ranges.

### Test Inputs

```text
Temperature = 104.9°C
Temperature = 105.0°C
```

### Expected Results

```text
104.9°C → Warning → Yellow LED
105.0°C → Critical → Red LED
```

### Result

**PASS**

---

## TC-05 — Critical-Low Boundary

**Requirements:** `LED-1`, `LED-3`, `LED-5`

### Objective

Verify the lower critical temperature boundary.

### Test Inputs

```text
Temperature = 5.0°C
Temperature = 4.9°C
```

### Expected Results

```text
5.0°C → OK → Green LED
4.9°C → Critical → Red LED
```

### Result

**PASS**

---

## TC-06 — Exactly One LED Active

**Requirements:** `SYS-1`, `HW-2`, `LED-4`

### Objective

Verify that exactly one LED is active for each valid temperature state.

### Test Conditions

```text
OK       → Green = ON, Yellow = OFF, Red = OFF
Warning  → Green = OFF, Yellow = ON, Red = OFF
Critical → Green = OFF, Yellow = OFF, Red = ON
```

### Expected Result

For every valid temperature classification:

```text
Number of active LEDs = 1
```

### Verification

The GPIO mock shall be inspected by the automated unit test.

### Result

**PASS**

---

## TC-07 — EEPROM Hardware Revision Configuration

**Requirements:** `HW-3`, `HW-4`, `TEMP-3`, `SW-4`

### Objective

Verify that the hardware revision read from EEPROM selects the correct temperature conversion.

### Test Inputs

```text
EEPROM revision = 0
Raw ADC = 10

EEPROM revision = 1
Raw ADC = 100
```

### Expected Results

```text
Revision 0 → Rev-A → 10°C
Revision 1 → Rev-B → 10°C
```

The correct sensor resolution shall be selected automatically based on EEPROM configuration.

### Result

**PASS**

---

## TC-08 — EEPROM Serial Number

**Requirements:** `HW-3`, `SW-4`

### Objective

Verify that the hardware serial number is read from EEPROM and preserved correctly.

### Test Input

```text
Serial number = "ABC1234"
```

### Expected Result

```text
Read serial number = "ABC1234"
```

The serial number is configuration data and is used as a placeholder as defined by the task.

### Result

**PASS**

---

## TC-09 — ISR Minimal Deterministic Work

**Requirements:** `SW-1`, `SW-2`, `HW-5`

### Objective

Verify that the 100 µs ISR performs only the time-critical sampling operations.

### Expected ISR Operations

```text
Timer interrupt
      |
      +--> ADC read
      |
      +--> Latch raw ADC value
      |
      +--> Set sample-ready indication
      |
      +--> Return
```

### ISR shall not perform

```text
Temperature conversion
Temperature classification
LED control
EEPROM access
Long-running processing
```

### Verification Method

Code inspection.

The PC demonstration cannot prove actual interrupt jitter or timing accuracy.

### Result

**PASS**

---

## TC-10 — 100 µs ISR Entry Point

**Requirements:** `SW-1`, `SW-5`, `HW-5`

### Objective

Verify that the required timer ISR function is implemented and can be invoked without physical hardware.

### Expected

The implementation shall provide a callable timer ISR entry point.

C:

```text
ISR_Timer100us_Handler()
```

C++:

```text
isr_timer_100us_handler()
```

The ISR shall invoke the corresponding temperature-monitoring sampling functionality.

### Verification

Automated test invokes the ISR entry point directly.

### Result

**PASS**

---

## TC-11 — Invalid Hardware Revision

**Requirements:** `TEMP-4`, `ERR-1`, `ERR-3`

### Objective

Verify that unsupported hardware revisions are rejected.

### Test Input

```text
Hardware revision = Invalid / unsupported value
```

### Expected Result

Sensor initialization shall fail safely.

The software shall not silently select Rev-A or Rev-B for an unsupported revision.

### Result

**PASS**

---

## TC-12 — NULL Sensor Pointer

**Requirement:** `ERR-2`

### Objective

Verify safe handling of a NULL sensor pointer during C sensor initialization.

### Test Input

```text
Sensor pointer = NULL
```

### Expected Result

The initialization function shall detect the invalid pointer and return an appropriate failure indication without dereferencing the pointer.

### Result

**PASS**

---

## TC-13 — C and C++ PC Demonstrations

**Requirements:** `SYS-1`, `SYS-3`, `SYS-4`, `SW-6`, `SW-7`, `SW-8`

### Objective

Verify that both implementations can be built and executed on a PC without physical hardware.

### Verification

The C project shall:

* Compile successfully.
* Use mocked hardware interfaces.
* Execute the temperature monitoring flow.
* Demonstrate temperature classification.
* Demonstrate LED behavior.

The C++ project shall:

* Compile successfully.
* Use mock implementations of hardware interfaces.
* Execute the equivalent monitoring flow.
* Demonstrate temperature classification.
* Demonstrate LED behavior.

### Expected Result

Both implementations shall execute successfully using PC-based mocked hardware.

### Result

**PASS**

---

## TC-14 — C/C++ Behavioral Comparison

**Requirements:** `SYS-1`, `SYS-4`, `SW-2`, `SW-3`, `SW-7`

### Objective

Verify that the C and C++ implementations provide equivalent functional behavior.

### Test Sequence

The same logical input sequence shall be applied to both implementations.

Example:

```text
Rev-A:
10°C
84.9°C
85.0°C
104.9°C
105.0°C
4.9°C

Rev-B:
10°C
84.9°C
85.0°C
104.9°C
105.0°C
4.9°C
```

### Expected Result

For each input:

```text
C result == C++ result
```

The following shall be equivalent:

* Temperature conversion
* Temperature classification
* LED selection

### Result

**PASS**

---

## TC-15 — Clean Compilation and Warning Verification

**Requirements:** `SYS-4`, `SW-6`, `SW-7`

### Objective

Verify that both implementations compile cleanly using the required compiler warnings.

### Required C flags

```text
-Wall -Wextra -Wpedantic
```

### Required C++ flags

```text
-Wall -Wextra -Wpedantic
```

### Expected Result

Both projects shall compile successfully without compiler warnings.

### Result

**PASS**

---

## TC-16 — Architecture Verification

**Requirements:** `SYS-2`, `HW-1` through `HW-5`, `SW-1` through `SW-8`

### Objective

Verify that the architecture documentation accurately represents the implementation.

### Architecture views reviewed

```text
00_Overall_architecture.png
01_system_static_architecture.png
02_hardware_static_architecture.png
05_software_static_architecture_cpp.png
06_software_dynamic_architecture_cpp.png
03_software_static_architecture_c.png
04_software_dynamic_architecture_c.png
```

### Review criteria

The architecture shall correctly represent:

* Temperature sensor
* ADC interface
* MCU
* Timer
* GPIO
* Three LEDs
* EEPROM
* C software architecture
* C++ software architecture
* Hardware abstraction
* Sensor resolution selection
* Temperature classification
* ISR responsibilities
* Deferred application processing
* C/C++ implementation separation

### Expected Result

Architecture diagrams shall be consistent with the requirements and implementation.

### Result

**PASS**

---

## TC-17 — Repository and Clean-Clone Verification

**Requirement:** `SYS-5`

### Objective

Verify that the final public GitHub repository contains the complete project and can be built from a clean clone.

### Repository shall contain

```text
README.md
REQUIREMENTS.md
TEST_PLAN.md
c/
cpp/
docs/
```

The `docs/` directory shall contain:

```text
diagrams.md
diagrams/
```

The diagrams directory shall contain all required architecture diagrams.

### Clean-clone verification

The repository shall be cloned into a clean directory.

Both implementations shall then be built from the clean clone using their documented build commands.

### Expected Result

* Repository is public.
* Source code is present.
* Tests are present.
* Requirements are present.
* Test plan is present.
* Architecture documentation is present.
* Architecture diagrams are present.
* C project builds successfully.
* C++ project builds successfully.
* Tests execute successfully.

### Result

**PASS**

---

# 5. Test Coverage Summary

| Area                   | Covered Test Cases  |
| ---------------------- | ------------------- |
| Temperature conversion | TC-01, TC-02        |
| Temperature boundaries | TC-03, TC-04, TC-05 |
| LED behavior           | TC-06               |
| EEPROM configuration   | TC-07, TC-08        |
| ISR architecture       | TC-09, TC-10        |
| Error handling         | TC-11, TC-12        |
| PC demonstration       | TC-13               |
| C/C++ equivalence      | TC-14               |
| Build quality          | TC-15               |
| Architecture           | TC-16               |
| Repository             | TC-17               |

---

# 6. Verification Method Summary

| Verification Method             | Test Cases                     |
| ------------------------------- | ------------------------------ |
| Automated unit test             | TC-01 to TC-08, TC-10 to TC-12 |
| Code inspection                 | TC-09                          |
| Build and execution             | TC-13                          |
| Manual behavioral comparison    | TC-14                          |
| Build verification              | TC-15                          |
| Manual architecture review      | TC-16                          |
| Repository/process verification | TC-17                          |

---

# 7. Target Hardware Verification

The PC demonstration does not verify real-time MCU characteristics.

When the C implementation is ported to the target microcontroller, the following measurements shall be performed:

| Target Verification | Objective                                                                |
| ------------------- | ------------------------------------------------------------------------ |
| Timer period        | Verify nominal 100 µs period                                             |
| Interrupt jitter    | Verify sampling timing variation is within the system timing requirement |
| ADC sampling timing | Verify ADC acquisition occurs at the intended sampling event             |
| ISR execution time  | Verify ISR completes within the available 100 µs budget                  |
| Timer configuration | Verify MCU timer is configured for the required periodic interrupt       |

These measurements are outside the scope of the PC demonstration because a general-purpose PC operating system cannot provide deterministic bare-metal interrupt timing.

---

# 8. Pass/Fail Criteria

A test case is considered **PASS** when the observed behavior satisfies its defined expected result.

A test case is considered **FAIL** when the implementation does not satisfy the expected result.

A test case is considered **PENDING** when the required verification activity has not yet been performed.

The final repository shall not be considered fully verified until `TC-17` has been completed successfully.

---

# 9. Overall Verification Status

| Verification Area             | Status                   |
| ----------------------------- | ------------------------ |
| Temperature conversion        | PASS                     |
| Temperature classification    | PASS                     |
| LED control                   | PASS                     |
| EEPROM configuration          | PASS                     |
| ISR implementation            | PASS                     |
| Error handling                | PASS                     |
| C PC demonstration            | PASS                     |
| C++ PC demonstration          | PASS                     |
| C/C++ behavioral comparison   | PASS                     |
| Compiler warning verification | PASS                     |
| Architecture review           | PASS                     |
| Public GitHub repository      | PASS                     |
| Clean-clone verification      | PASS                     |
| Target MCU timing/jitter      | Target hardware activity |

---

# 10. Traceability Statement

The project maintains bidirectional verification traceability:

**Requirement → Test Case → Verification Result**

and:

**Test Case → Requirement**

The complete engineering flow is:

**Requirement → Architecture → Implementation → Test → Verification**

The C implementation provides the bare-metal-oriented embedded implementation.

The C++ implementation provides equivalent functionality using object-oriented design principles.

The PC demonstrations use mocked hardware interfaces to verify functional behavior without physical target hardware.

Actual MCU-specific timing characteristics, including 100 µs timer accuracy, interrupt jitter, ADC sampling timing, and ISR execution time, remain target-hardware verification activities.

---

**End of Test Plan**
