# ADR-0001: ESP32 as the real-time central controller

- **Status:** Accepted as the initial architecture baseline; implementation details remain provisional.
- **Date:** 2026-10-18
- **Decision owners:** RailDrone project team

## Context

RailDrone is a rail-mounted aerial/mechatronic platform expected to control vertical position and angular orientation using two propulsion channels. The system needs local sensor acquisition, time-sensitive control, actuator command generation, safety logic, operating-state management, gamepad processing, and telemetry. An external dashboard is useful for monitoring and configuration but depends on a communication link with variable availability and timing.

Hardware selection is incomplete. The ESP32 DevKit is the baseline controller, the BNO085 is likely, and the remaining sensors, propulsion hardware, power components, and command protocols are not frozen.

## Decision

Use the ESP32 DevKit (`esp32dev`, Arduino framework) as the real-time central controller. Keep sensor acquisition, real-time control, safety logic, state machine, actuator commands, gamepad processing, and core telemetry on the ESP32.

Limit the external dashboard to supervisory monitoring and configuration. It must not perform real-time stabilization, own safety decisions, or be required for local actuator inhibition. Dashboard-originated configuration or commands, if later supported, remain subordinate to local validation, interlocks, and safety authority.

## Consequences

- The local controller can continue its safety responsibilities when the dashboard connection is unavailable.
- Timing-sensitive control and actuator paths do not depend on network latency or dashboard execution.
- The ESP32 firmware must provide clear interfaces among sensing, normalized state, input/reference handling, control, mixer, actuation, safety, state machine, and telemetry.
- Wi-Fi telemetry and Bluetooth gamepad operation may compete for radio resources and must be validated on the selected module and firmware.
- ESP32 compute, memory, peripheral, and real-time limits must be checked against the eventual sensor rates and control requirements.
- Hardware choices and detailed safety behavior remain open. This ADR does not select an ESC protocol, power design, control law, or motor mixer.

## Alternatives considered

1. **External dashboard as the real-time controller:** Rejected because network and host availability/timing cannot be the foundation for stabilization or safety.
2. **A separate real-time controller plus ESP32 communications gateway:** Deferred. It may be reconsidered if measured timing, safety, or resource requirements exceed the selected ESP32's capability.

## References

- [System architecture baseline](../architecture/system-architecture.md)
- [Safety state machine baseline](../architecture/safety-state-machine.md)
- [Telemetry baseline](../protocols/telemetry-baseline.md)
