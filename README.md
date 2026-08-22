# Temperature Monitoring and Visualization System

## Project Overview

This project implements a bare-metal embedded software architecture for a temperature monitoring and visualization device.

The system reads temperature data from an ADC-based temperature sensor, processes the measured temperature, and indicates the temperature condition using three GPIO-controlled LEDs:

* **Green:** Normal operation — temperature < 85°C
* **Yellow:** Warning — temperature ≥ 85°C and < 105°C
* **Red:** Critical — temperature ≥ 105°C or < 5°C

The system supports two hardware revisions with different temperature sensor resolutions:

* **Rev-A:** 1 digit = 1°C
* **Rev-B:** 1 digit = 0.1°C

The hardware revision and hardware serial number are obtained from EEPROM configuration during initialization.

---

## Documentation & Deliverables

| File | Purpose |
|---|---|
| [`Interview_Walkthrough_Presentation.pptx`](Interview_Walkthrough_Presentation.pptx) / [`.pdf`](Interview_Walkthrough_Presentation.pdf) | Interview presentation — task, approach, architecture, design decisions, actual demo/test console output, requirements coverage. |
| [`docs/diagrams.md`](docs/diagrams.md) | All architecture diagrams embedded inline, with Mermaid source alongside. |
| [`docs/diagrams/`](docs/diagrams) | The diagrams as standalone `.png` files (and their `.mermaid` source). |
| [`docs/REQUIREMENTS.md`](docs/REQUIREMENTS.md) | Full requirement list with IDs. |
| [`docs/TEST_PLAN.md`](docs/TEST_PLAN.md) | TC-01..TC-17, detailed steps, expected results, traceability. |

---

## Architecture Overview

The architecture separates time-critical sampling from application-level temperature processing.

The **100 µs timer interrupt** performs only minimal deterministic work:

1. Trigger ADC sampling
2. Read the raw ADC value
3. Latch the sampled value
4. Set a sample-ready flag
5. Return from the interrupt

Temperature conversion, classification, and LED control are performed in the application/main-loop context.

This prevents non-time-critical processing from unnecessarily increasing interrupt execution time.

> **Verification boundary:** The PC demonstration verifies functional behaviour using mocked hardware interfaces. Actual MCU-level timer accuracy, interrupt jitter, ADC timing, and ISR execution time require target-hardware verification.

---

## Project Structure

```text
Temperature-Monitoring-and-Visualization-System/
├── README.md
├── c/
│   ├── include/
│   ├── src/
│   └── tests/
├── cpp/
│   ├── include/
│   ├── src/
│   └── tests/
└── docs/
    ├── diagrams/
    ├── diagrams.md
    ├── REQUIREMENTS.md
    └── TEST_PLAN.md
```

---

## Key Requirements

* ADC-based temperature measurement
* Temperature sampling every 100 µs
* Deterministic sampling architecture
* GPIO-based temperature status indication
* Support for Rev-A and Rev-B temperature sensors
* EEPROM-based hardware configuration
* PC-based demonstration using mocked hardware interfaces
* C implementation
* C++ implementation using object-oriented programming principles
* Static and dynamic architecture documentation
* Requirement-to-architecture-to-test traceability

---

## Implementations

The project contains two independent implementations providing equivalent functional behaviour.

### C Implementation

The C implementation uses a modular procedural architecture with separation between application logic and hardware abstraction.

The C implementation includes:

* Timer ISR
* ADC abstraction
* Temperature conversion
* Temperature classification
* LED control
* EEPROM configuration
* ISR-to-main-loop sample transfer
* Mock hardware interfaces for PC demonstration

The interrupt path is intentionally kept short. Temperature conversion, classification, and LED processing are performed outside the ISR.

### C++ Implementation

The C++ implementation provides the same functional behaviour using object-oriented design principles.

It uses:

* Hardware interfaces such as `IAdc`, `IGpio`, and `IEeprom`
* Dependency injection for hardware dependencies
* Encapsulation of monitoring state
* `TemperatureMonitor` for sampling and application processing
* `TemperatureSensor` for temperature conversion
* `TemperatureClassifier` for status determination
* `LedController` for GPIO LED control
* Strategy-based sensor resolution for Rev-A and Rev-B
* Mock hardware interfaces for PC demonstration

---

## Temperature Processing

```text
                 ADC Raw Value
                       |
                       v
              Hardware Revision
                       |
              +--------+--------+
              |                 |
           Rev-A             Rev-B
              |                 |
        1 °C / digit       0.1 °C / digit
              |                 |
              +--------+--------+
                       |
                       v
                Temperature °C
                       |
                       v
             Temperature Classifier
                       |
          +------------+------------+
          |            |            |
          v            v            v
         OK         WARNING      CRITICAL
          |            |            |
        Green        Yellow         Red
```

Exactly one temperature-status LED is intended to be active at a time.

### Temperature Classification

| Temperature        | Status   | LED    |
| ------------------ | -------- | ------ |
| `< 5°C`            | Critical | Red    |
| `5°C ≤ T < 85°C`   | OK       | Green  |
| `85°C ≤ T < 105°C` | Warning  | Yellow |
| `≥ 105°C`          | Critical | Red    |

---

## Hardware Architecture

The target hardware consists of:

| Hardware           | Interface          | Purpose                                    |
| ------------------ | ------------------ | ------------------------------------------ |
| Temperature Sensor | Analog             | Temperature measurement                    |
| ADC                | MCU peripheral     | Converts analog signal to digital value    |
| Timer              | Interrupt          | Generates 100 µs sampling event            |
| GPIO               | Digital output     | Controls status LEDs                       |
| EEPROM             | I2C                | Stores hardware revision and serial number |
| MCU                | Central controller | Executes embedded software                 |

### Hardware Revisions

| Revision |      Resolution |
| -------- | --------------: |
| Rev-A    |   1°C per digit |
| Rev-B    | 0.1°C per digit |

The active sensor resolution is selected from the hardware revision stored in EEPROM.

---

## Interrupt and Main-Loop Architecture

The system separates sampling from application processing.

### Interrupt Context

```text
100 µs Timer
     |
     v
   Timer ISR
     |
     +--> Read ADC
     |
     +--> Latch raw value
     |
     +--> Set sample-ready flag
     |
   Return
```

### Application Context

```text
Application / Main Loop
          |
          v
 Check sample-ready flag
          |
          v
   Read latched value
          |
          v
 Convert raw value to °C
          |
          v
 Classify temperature
          |
          v
 Update status LED
```

The ISR does not perform:

* Floating-point temperature conversion
* Temperature classification
* LED processing
* Other non-time-critical application logic

This keeps the interrupt path short and deterministic.

---

## PC Demonstration

The project provides a PC-based demonstration using mocked hardware interfaces.

The PC environment allows the functional software behaviour to be demonstrated without physical target hardware.

The mocked interfaces include:

* Mock ADC
* Mock GPIO
* Mock EEPROM

The PC demonstration is intended for functional verification and does **not** emulate the electrical behaviour or real-time characteristics of the MCU peripherals.

---

## Architecture Documentation

The `docs/` directory contains the architecture and verification documentation.

The architecture documentation provides the following seven views:

1. **Overall System Architecture**
2. **System Static Architecture**
3. **Hardware Static Architecture**
4. **C Software Static Architecture**
5. **C Software Dynamic Architecture**
6. **C++ Software Static Architecture**
7. **C++ Software Dynamic Architecture**

### Architecture Diagram Files

```text
docs/
└── diagrams/
    ├── 00_Overall_architecture.png
    ├── 01_system_static_architecture.png
    ├── 02_hardware_static_architecture.png
    ├── 03_software_static_architecture_c.png
    ├── 04_software_dynamic_architecture_c.png
    ├── 05_software_static_architecture_cpp.png
    └── 06_software_dynamic_architecture_cpp.png
```

### Static Architecture

The static views describe:

* System structure
* Hardware interfaces
* Software components
* Dependencies
* Hardware abstraction
* C and C++ software organization

### Dynamic Architecture

The dynamic views describe:

* 100 µs timer interrupt
* ADC sampling
* Raw-value latching
* Sample-ready signalling
* Main-loop processing
* Temperature conversion
* Temperature classification
* LED status update

---

## Design Principles

The architecture follows these principles:

### Separation of Concerns

Hardware access, temperature processing, classification, and LED control are separated into distinct components.

### Hardware Abstraction

Hardware interfaces are separated from application logic so that the software can be demonstrated and tested using mocked hardware.

### Deterministic Interrupt Processing

The timer ISR performs only the minimum required sampling and state-transfer operations.

### C Modular Design

The C implementation uses modular functions and hardware abstraction interfaces.

### C++ Object-Oriented Design

The C++ implementation uses:

* Abstraction
* Encapsulation
* Interfaces
* Dependency injection
* Polymorphism
* Strategy pattern

### Hardware Revision Strategy

Rev-A and Rev-B temperature conversion behaviour is selected according to the hardware configuration.

---

## EEPROM Configuration Flow

The hardware configuration is read during system initialization.

```text
System Startup
      |
      v
Initialize EEPROM
      |
      v
Read Hardware Configuration
      |
      +----------------------+
      |                      |
Hardware Revision       Serial Number
      |                      |
      v                      v
Select Resolution       Store Configuration
      |
      +---- Rev-A -> 1°C/digit
      |
      +---- Rev-B -> 0.1°C/digit
      |
      v
Initialize Temperature Monitoring
```

An invalid hardware revision is rejected during initialization.

---

## Error Handling

The architecture includes explicit validation for invalid configuration and invalid C API usage.

### Invalid Hardware Revision

```text
EEPROM
  |
  v
Read Hardware Revision
  |
  +---- Rev-A ----> Valid
  |
  +---- Rev-B ----> Valid
  |
  +---- Other ----> Initialization Failure
```

### NULL Pointer Validation

The C temperature-sensor initialization API validates the sensor pointer before accessing it.

```text
TempSensor_Init(sensor)
        |
        v
   sensor == NULL ?
       /       \
     YES       NO
      |         |
   Failure    Continue
```

---

## Verification Approach

The verification approach covers:

* ADC value processing
* Hardware revision selection
* Temperature conversion
* Temperature classification
* LED status control
* EEPROM configuration handling
* Invalid configuration handling
* NULL pointer handling in C
* ISR sampling behaviour
* C/C++ functional equivalence
* PC build and execution

### Target-Hardware Verification Boundary

The following require verification on actual target hardware:

* Actual 100 µs timer accuracy
* Interrupt jitter
* ADC sampling timing
* ISR execution time
* MCU-specific timer configuration
* MCU-specific ADC behaviour
* Actual GPIO electrical behaviour
* Actual I2C EEPROM communication

Therefore, successful PC execution must not be interpreted as proof of actual MCU-level timing accuracy.

---

## Requirements and Test Traceability

The architecture provides traceability through the development flow:

```text
Requirement
     |
     v
Architecture
     |
     v
Component Design
     |
     v
Implementation
     |
     v
Test
     |
     v
Verification Result
```

The documentation provides traceability between:

**Requirement → Architecture → Component → Runtime Behaviour**

and:

**Requirement → Design → Implementation → Test**

See [`docs/REQUIREMENTS.md`](docs/REQUIREMENTS.md) for the requirements.

See [`docs/TEST_PLAN.md`](docs/TEST_PLAN.md) for the verification strategy and test cases.

See [`docs/diagrams.md`](docs/diagrams.md) for the complete architecture documentation.

---

## Project Status

The project contains:

* C implementation
* C++ implementation
* Hardware abstraction and mock interfaces
* Architecture documentation
* Static architecture diagrams
* Dynamic architecture diagrams
* PC-based functional demonstration
* Verification and test documentation

The implementation, tests, and documentation may continue to evolve as additional verification and target-hardware activities are performed.

---

## Final Architecture Summary

The Temperature Monitoring and Visualization System follows a layered architecture:

```text
                     TEMPERATURE MONITORING SYSTEM
                                |
              +-----------------+-----------------+
              |                                   |
           HARDWARE                            SOFTWARE
              |                                   |
      +-------+-------+                   +-------+-------+
      |       |       |                   |               |
   Sensor   Timer    MCU                 C              C++
      |       |       |                   |               |
     ADC    ISR     GPIO              Procedural         OOP
                      |                   |               |
                   EEPROM              Mock HAL       Interfaces
                      |                   |               |
                    LEDs              PC Demo         PC Demo
                      |                   |               |
                      +-------------------+---------------+
                                          |
                                   Temperature Processing
                                          |
                                   Classification
                                          |
                                   Green / Yellow / Red
```

The overall design follows:

**Requirement → Architecture → Implementation → Test**

with a clear separation between:

**Time-Critical Sampling → Deferred Application Processing**

and between:

**Target Hardware → Hardware Abstraction → Application Logic**

This architecture allows the same functional temperature-monitoring behaviour to be implemented in both C and C++ while supporting PC-based demonstration and structured verification.
