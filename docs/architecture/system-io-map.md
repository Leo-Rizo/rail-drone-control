# RAIL-F01 — System I/O Map

All pin assignments in this document are **PROVISIONAL** examples for a conventional ESP32 DevKit using a classic ESP32 module. Verify the exact DevKit schematic, module variant, board routing, selected peripherals, and boot behavior before wiring. This document does not authorize powered motor operation.

## Hardware interface matrix

Rates are preliminary design targets or expected ranges, not implemented schedules. Final rates depend on component selection, bus loading, control timing, and validation.

| Subsystem | Component | ESP32 interface | Direction | Expected update rate | Safety relevance | Current status | Notes |
|---|---|---|---|---|---|---|---|
| Orientation | BNO085 IMU | Shared I2C SDA/SCL; optional interrupt GPIO TBD | Input | 50–200 Hz target, TBD by report/configuration | High | PROVISIONAL; part likely | Confirm breakout voltage, I2C address, interrupt use, and sensor report mode. |
| Height | Time-of-Flight sensor | Shared I2C SDA/SCL; optional XSHUT/interrupt TBD | Input | 20–100 Hz target, TBD by model | High | TBD | Exact sensor/model, address, field of view, range, and timing are not selected. |
| Travel boundary | Upper limit switch | GPIO input with defined external/pull-up bias | Input | 50–200 Hz sampled/debounced target | High | PROVISIONAL | Contact type, fail-safe wiring, debounce, and open/short detection TBD. |
| Travel boundary | Lower limit switch | GPIO input with defined external/pull-up bias | Input | 50–200 Hz sampled/debounced target | High | PROVISIONAL | Contact type, fail-safe wiring, debounce, and open/short detection TBD. |
| Power health | Battery voltage monitor | ADC input through TBD measurement/protection network | Input | 1–10 Hz target | High | PROVISIONAL | Divider, filtering, ADC calibration, battery chemistry/range and thresholds TBD. |
| Propulsion | ESC channel 1 | PWM-capable GPIO; exact protocol TBD | Output | 50–500 Hz target, protocol-dependent | Critical | TBD | Do not infer pulse format, neutral, arming, failsafe, or stop behavior. |
| Propulsion | ESC channel 2 | PWM-capable GPIO; exact protocol TBD | Output | 50–500 Hz target, protocol-dependent | Critical | TBD | Must be coordinated with ESC 1 timing and safety inhibition. |
| Propulsion | BLDC motor 1 | Driven by ESC 1; no direct ESP32 interface | Output path | ESC-dependent | Critical | TBD | Motor, propeller, rotation direction, and mechanical installation TBD. |
| Propulsion | BLDC motor 2 | Driven by ESC 2; no direct ESP32 interface | Output path | ESC-dependent | Critical | TBD | Motor, propeller, rotation direction, and mechanical installation TBD. |
| Operator input | PlayStation controller | Bluetooth (controller profile/library TBD) | Input | 20–100 Hz target, device-dependent | High | PROVISIONAL | Connection loss, stale input, pairing, and neutral-on-disconnect behavior TBD. |
| Development/debug | USB Serial | USB-to-UART / UART0 as provided by DevKit | Bidirectional | Event/log driven; TBD | Medium | PROVISIONAL | Keep GPIO1/GPIO3 and USB circuitry available for programming/debugging. |
| Dashboard link | Wireless telemetry | Wi-Fi radio; protocol TBD | Bidirectional, telemetry output primary | 1–20 Hz target | Low for stabilization; configuration access requires controls | PROVISIONAL | Dashboard remains supervisory; Wi-Fi loss cannot own safety behavior. |

## Provisional ESP32 pin map

| Signal | Proposed ESP32 pin | Direction | Status | Rationale / constraints |
|---|---:|---|---|---|
| I2C SDA (BNO085 + ToF) | GPIO21 | Bidirectional | PROVISIONAL | Common default on many ESP32 DevKit boards; one shared bus. Confirm board and module routing. |
| I2C SCL (BNO085 + ToF) | GPIO22 | Output clock | PROVISIONAL | Common default on many ESP32 DevKit boards; one shared bus. Confirm bus pull-ups and voltage. |
| Upper limit switch | GPIO32 | Input | PROVISIONAL | Input-capable; use a defined bias and verify wiring/fail-safe state. |
| Lower limit switch | GPIO33 | Input | PROVISIONAL | Input-capable; use a defined bias and verify wiring/fail-safe state. |
| Battery sense | GPIO34 / ADC1 | Input only | PROVISIONAL | ADC1 avoids classic ESP32 ADC2/Wi-Fi resource contention. Conditioning network and range TBD. |
| ESC 1 signal | GPIO25 | Output | PROVISIONAL | Output-capable; final timer/protocol and board conflicts require validation. |
| ESC 2 signal | GPIO26 | Output | PROVISIONAL | Output-capable; final timer/protocol and board conflicts require validation. |
| BNO085 interrupt | TBD | Input | TBD | Assign only if selected breakout/report configuration requires it. |
| ToF shutdown/interrupt | TBD | GPIO | TBD | Depends on exact ToF module and address-management needs. |
| Physical emergency stop input | TBD | Input and/or independent hardware path | TBD | Must be designed with the hardware interruption path; a firmware pin alone is not an independent power disconnect. |
| USB Serial | GPIO1 TX / GPIO3 RX where exposed | Bidirectional | Board-dependent | Reserved for programming/debug where these pins are used by the DevKit USB-UART bridge. |

The proposed pins avoid the commonly used classic ESP32 strapping pins GPIO0, GPIO2, GPIO5, GPIO12, and GPIO15 for these example assignments. This is not a guarantee for every ESP32 variant or DevKit. GPIO6–GPIO11 are commonly connected to module flash and are not proposed. Recheck all assignments against the exact board documentation before freezing the map.

## Shared bus and electrical interface notes

- BNO085 and ToF are provisionally on the same I2C bus. Confirm unique addresses; if they conflict, select a supported address option or a bus mux after the ToF model is known.
- ESP32 GPIO is not a substitute for level shifting or input protection. Establish compatible logic levels and a common reference only after selecting modules and power domains.
- Place pull-ups only as required by the combined bus; account for module-integrated pull-ups and cable capacitance.
- Keep noisy ESC/motor wiring physically separated from sensor and ADC wiring. Use appropriate filtering and routing in the detailed design.
- All pin assignments, rates, protocols, and thresholds are revision-controlled design inputs and must be updated when hardware is selected.
