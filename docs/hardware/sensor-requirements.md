# RAIL-F02 — Sensor and Operator-Input Requirements

## BNO085 IMU

The BNO085 remains the likely IMU from RAIL-F01. Retain the assumption unless a documented interface, range, rate, latency, environmental, or mounting incompatibility is found. Confirm the exact device/breakout before electrical design.

Required capabilities and evidence:

- Orientation output in a documented frame and representation, with range/accuracy and validity behavior suitable for the required angular envelope.
- Angular velocity output if needed by the eventual controller; confirm units, range, noise, bandwidth, and report configuration.
- Update rate and end-to-end latency adequate for required angular response; the existing I/O map's 50–200 Hz is a provisional target, not a requirement freeze.
- Communication bus and electrical levels compatible with ESP32; I2C is provisional and shared-bus addresses, pull-ups, cable capacitance, and recovery must be checked. Confirm any interrupt/data-ready needs.
- Rigid, documented mounting orientation and coordinate transform; minimize vibration transmission and keep away from magnetic/noisy-current sources as recommended by the device guidance.
- Calibration procedure, calibration status reporting, startup validity, recovery behavior, and recalibration criteria.
- Vibration sensitivity and installed performance verified with motors/ESCs in controlled tests before relying on measurements.

## ToF height sensor

Exact ToF model remains TBD. Select only after measuring usable vertical travel and evaluating target surfaces, mounting, lighting, and update needs.

| Requirement | Definition needed |
|---|---|
| Minimum distance | Nearest sensor-to-target distance across travel and tolerances. |
| Maximum distance | Farthest distance across travel and tolerances, with margin to specified operating range. |
| Resolution / repeatability | Height increment, noise, accuracy, and repeatability required by the eventual control tolerance; values TBD. |
| Update rate / latency | Fresh measurement rate and latency matched to control response; existing 20–100 Hz map is provisional only. |
| Field of view | Narrow/appropriate enough to avoid rail, carriage, propellers, and background returns throughout motion. |
| Ambient light | Reliable operation under expected indoor/outdoor and direct-light conditions; validate with target surfaces. |
| Target surface | Test reflectivity, color, angle, texture, size, and contamination/wear of the actual measurement target. |
| Interface | ESP32-compatible bus/protocol, I2C address and sharing compatibility or alternate interface; interrupt/shutdown needs TBD. |
| Voltage | Exact module input and logic-level range compatible with the regulated rail; no voltage assumed. |
| Physical fit | Module dimensions, mounting, optical aperture, alignment, protection, cable, and full-travel clearance. |

## Limit switches

Upper and lower travel boundary sensing is present in the architecture. Exact switch type, contact behavior, and part remain TBD. Choose a device with mechanical/electrical ratings appropriate to the physical environment and input circuit; define repeatability, overtravel, mounting rigidity, actuator alignment, and access for inspection.

- Decide normally-open versus normally-closed from a documented fault model (broken wire, short, connector loss, stuck actuator), input diagnostics, and wiring practicality; neither is selected here without that analysis.
- Place switches before hazardous mechanical stops with measured allowance for sensing, debounce, command/actuator delay, and stopping distance. Do not let a switch act as the routine impact stop.
- Define sampled/debounced behavior and response to contradictory, stuck, open/short, or stale inputs. Debounce must not mask a safety event beyond the stopping allowance.
- Determine internal versus external pull requirements from the exact ESP32 pin and circuit. Provide external bias/protection if required by noise, cable length, or fault-detection design; do not rely on unsupported internal pulls.

## Gamepad requirements

The architecture expects a Bluetooth PlayStation controller, but exact generation, profile, and implementation library remain TBD. Identify supported controller generations and pairing behavior using the intended ESP32/Bluetooth stack. Define the required input update rate from the operator-command needs (existing 20–100 Hz I/O-map range is provisional), timestamp/stale-input timeout, disconnect detection, and deterministic safe command on disconnect. Define deadband per axis/button based on measured controller behavior and intended function, plus command mapping, scaling, limits, and neutral behavior. Gamepad commands must remain subject to local safety state and must not bypass it. Verify reconnection cannot unexpectedly re-arm propulsion.
