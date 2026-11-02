# RAIL-F03A — Firmware Architecture Skeleton

## Purpose and scope

The ESP32 DevKit firmware provides the local boundary for sensor acquisition, normalized state, operator input, safety, operating state, future control, actuator requests, and telemetry. This skeleton establishes data contracts and testable logic only. Hardware drivers, control algorithms, physical ESC output, and telemetry transport are not implemented.

The EX1103 motors and existing propellers remain provisional. Their prototype observation does not validate the estimated final moving mass. Propulsion selection and characterization remain outside this firmware task.

## Module responsibilities

| Module | Responsibility |
|---|---|
| `core` | Shared SI-unit data types, operating/control modes, fault codes, and explicit state transitions. |
| `sensors` | Hardware-independent IMU, height, and limit-switch contracts. Implementations populate normalized values and health/validity. |
| `input` | Gamepad contract that produces a library-independent `OperatorCommand`. |
| `control` | Future controller boundary and provisional `MotorMixer`; no PID behavior is present. |
| `actuators` | Normalized `MotorCommands` output contract; ESC protocol remains TBD. |
| `safety` | Applies operating-state inhibition, injected sensor/operator requirements, emergency-stop authority, and travel-limit direction checks. |
| `telemetry` | Builds an in-memory record from state, references, actuator requests, and fault state. Serialization and transport are absent. |
| `config` | Compile-time software constants. No unverified pins, sensor settings, gains, or electrical values are introduced. |

## Dependency direction

Hardware-specific implementations will depend on the abstract interfaces and normalized core types. Core data models do not depend on sensor or gamepad libraries. Control consumes normalized state and references. Safety is applied after mixer allocation and before any future actuator implementation. Telemetry observes the resulting state and safe actuator request.

```mermaid
flowchart LR
  Sensors[Sensor interfaces] --> State[Normalized SystemState]
  Gamepad[Gamepad interface] --> Reference[OperatorCommand / Reference]
  State --> Control[Future control layer]
  Reference --> Control
  Control --> Effort[ControlEffort]
  Effort --> Mixer[Provisional MotorMixer]
  Mixer --> Requested[MotorCommands]
  Requested --> Safety[SafetyManager]
  State --> Safety
  Mode[StateMachine authority] --> Safety
  Safety --> Actuator[IMotorOutput interface]
  State --> Telemetry[TelemetryRecord]
  Reference --> Telemetry
  Safety --> Telemetry
  Telemetry -. supervised by .-> Dashboard[External dashboard]
```

The dashboard remains supervisory. It does not participate in the real-time safety or actuator path.

## Normalized data contracts

- `SystemState` uses SI units and contains no device-library types. Numeric measurements have validity flags; default construction means unavailable data, not a zero measurement.
- `SystemMode` represents the operating lifecycle: `BOOT`, `SELF_TEST`, `DISARMED`, `ARMED`, `RUNNING`, `FAULT`, and `EMERGENCY_STOP`.
- `ControlMode` separately reserves `MANUAL`, `ASSISTED`, and `AUTOMATIC`; no automatic behavior is implemented.
- `OperatorCommand` carries arm/disarm/run/emergency requests, provisional normalized axes, references, and requested control mode without binding to a controller library.
- `ControlEffort` represents controller intent before motor allocation. Its upward sign convention is provisional.
- `MotorCommands` uses a normalized range from `0.0` (minimum/stopped request) through `1.0` (maximum request). This does not establish physical ESC safe behavior.
- `TelemetryRecord` follows the RAIL-F01 logical baseline and adds validity flags so unavailable values are distinguishable from numeric zero or inactive switch readings.

## State-machine ownership and authority

`StateMachine` starts in `BOOT` and exposes only named transition requests. No transitions occur implicitly in the loop. `ARMED` does not grant motor authority; only `RUNNING` does. Fault reset and emergency reset require explicit conditions and both return to `SELF_TEST` before further operation. Emergency stop is latched and has priority over ordinary state transitions.

`SafetyManager` is hardware-independent and locally evaluated. It inhibits output in every state except `RUNNING`, on requested emergency stop, on injected required-sensor or required-gamepad loss, and on an effort directed farther into an active limit. Since the final thrust mapping is unknown, a blocked direction conservatively inhibits both motor commands; a request away from the active limit can pass when the state and other interlocks allow it. Physical emergency power interruption remains a separate hardware design responsibility.

IMU, ToF, limit-switch, and operator-input requirements are passed explicitly to safety evaluation. No freshness windows, plausibility limits, battery thresholds, debounce values, or final fault thresholds are invented here. The caller can map returned faults to explicit state-machine fault transitions.

## Hardware abstraction boundaries

`IImu`, `IHeightSensor`, `ILimitSwitches`, `IGamepad`, and `IMotorOutput` define the replaceable integration seams. Implementations can later wrap a selected library or bus while keeping those types out of control and safety logic. No GPIO, ADC, Bluetooth, ESC, Wi-Fi, or dashboard implementation exists in this skeleton.

## Future extension points

1. Add selected sensor implementations that set values, validity, health, and monotonic timestamps.
2. Define the validated sensor freshness and arming/run interlocks from the hardware and safety review.
3. Add controller algorithms between `SystemState`/references and `ControlEffort`.
4. Characterize the plant, then revise/tune the provisional mixer and its sign conventions.
5. Select an ESC protocol and prove its safe/disarmed command behavior before implementing `IMotorOutput`.
6. Add telemetry serialization and a transport while retaining ESP32 ownership of safety and real-time behavior.
