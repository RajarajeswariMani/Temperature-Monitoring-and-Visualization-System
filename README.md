# Temperature Monitoring and Visualization System

## Project Overview

This project implements a bare-metal embedded software architecture for a temperature monitoring and visualization device.

The system reads temperature data from an ADC-based temperature sensor, processes the measured temperature, and indicates the temperature condition using three GPIO-controlled LEDs:

* **Green:** Normal operation — temperature < 85°C
* **Yellow:** Warning — temperature ≥ 85°C and < 105°C
* **Red:** Critical — temperature ≥ 105°C or < 5°C

The software supports two hardware revisions with different temperature sensor resolutions:

* **Rev-A:** 1 digit = 1°C
* **Rev-B:** 1 digit = 0.1°C

The hardware revision and hardware serial number are obtained from EEPROM configuration.

## Project Structure

```text
Temperature-Monitoring-and-Visualization-System/
│
├── README.md
├── c/
├── cpp/
└── docs/
```

### Directories

* `c/` — C implementation of the temperature monitoring system
* `cpp/` — C++ implementation using object-oriented programming principles
* `docs/` — Software architecture and design documentation

## Key Requirements

* ADC-based temperature measurement
* Temperature sampling every 100 μs
* Low-jitter sampling architecture
* GPIO-based temperature status indication
* Support for Rev-A and Rev-B temperature sensors
* EEPROM-based hardware configuration
* PC-based demonstration using mocked hardware interfaces
* C implementation
* C++ implementation using OOP principles
* Architecture documentation and diagrams

## Implementation

The project contains two implementations:

### C Implementation

The C implementation follows a modular embedded software architecture with hardware interfaces separated from application logic. Hardware-dependent functionality can be mocked for PC-based demonstration.

### C++ Implementation

The C++ implementation provides the same functional behavior while applying object-oriented programming concepts such as abstraction, encapsulation, interfaces, and polymorphism.

## Documentation

Architecture diagrams and supporting documentation are provided in the `docs/` directory.

## Status

The project is currently under development. The implementation, tests, and documentation will be added and updated as the project progresses.
