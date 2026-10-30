# RAIL-F02 — Hardware System Requirements Baseline

**Project:** RailDrone Control System

**Baseline:** Requirements and selection criteria; component selection remains open.

## Purpose and boundary

Define what the physical hardware must be capable of before selecting exact propulsion, sensing, or power components. No numeric performance values are invented here. The ESP32 remains the local real-time controller and safety authority; the dashboard remains supervisory. Two propulsion channels and the BNO085 are architectural assumptions from RAIL-F01, subject to verification. This document does not authorize firmware implementation or powered propeller tests.

## Status vocabulary

- **KNOWN** — explicitly established in the current architecture or repository; not necessarily physically validated.
- **TO MEASURE** — must be obtained from the physical prototype or a representative test.
- **TO CALCULATE** — derive from measured inputs and documented design targets/models.
- **TBD** — a design target, method, or decision has not yet been established.

Status describes present knowledge, not completion. A physically measured input can later be marked KNOWN with its evidence and revision.

## Mechanical inputs

| Parameter | Status | Required definition / evidence | Used for |
|---|---|---|---|
| Total moving mass | TO MEASURE | Weigh the complete moving assembly in its flight/test configuration; state included cables, guards, payload, and battery. Record instrument and uncertainty. | Vertical force and inertia model. |
| Estimated final mass | TO CALCULATE | Sum measured component and structure masses, then account for wiring, fasteners, guards, and unresolved items; update against assembled mass. | Preliminary sizing before final assembly. |
| Carriage mass | TO MEASURE | Weigh carriage and all parts translating with it, with inclusion boundary stated. | Rail support/friction and moving dynamics. |
| Propulsion assembly mass | TO MEASURE | Weigh motors, propellers, mounts, ESCs if carried, and associated moving wiring; state boundary. | Total mass, inertia, and envelope. |
| Center of mass | TO MEASURE | Locate in a defined coordinate frame for the fully assembled moving unit and relevant configurations. | Stability, moment arms, coupling, and inertial model. |
| Motor spacing | TO MEASURE | Measure motor thrust-axis locations and separation in the frame. | Effective angular moment arm and clearance. |
| Effective moment arm | TO CALCULATE | Determine perpendicular distance from relevant rotation axis to each thrust line, including geometry and center-of-mass location. | Differential-thrust torque. |
| Permitted vertical travel | TO MEASURE | Measure usable travel between mechanical endpoints and any restricted zones. | Sensor range, switch locations, motion limits. |
| Permitted angular travel | TBD | Define allowed angle from mechanism, stability, wiring, and safety constraints; verify mechanically. | Control envelope and stops. |
| Mechanical stops | TO MEASURE | Identify type, location, stiffness/impact behavior, and whether they constrain vertical and angular motion. | Safety envelope and force model. |
| Rail friction / resistance | TO MEASURE | Measure breakaway and running resistance over the travel, direction, speed, and representative loading; include uncertainty and method. | Net force and disturbance model. |
| Expected disturbances | TBD | Define credible external loads, impacts, cable drag, friction variation, and operating conditions. Quantify after measurement or hazard analysis. | Acceleration and rejection requirements. |
| Available physical envelope | TO MEASURE | Record mounting volumes and keep-out zones along travel, including wiring and service access. | Component packaging. |
| Propeller clearance | TO MEASURE | Check swept disks against rail, carriage, guards, structure, wiring, stops, and operator access over the full motion envelope. | Propeller size/safety feasibility. |
| Rotation-axis location and orientation | TO MEASURE | Define reference axes and relation to rail, thrust axes, and IMU axes. | Torque and sensor transforms. |
| Moment of inertia about controlled axis | TO CALCULATE | Calculate from measured mass distribution or measure with a documented pendulum/inertia method. | Angular acceleration and torque demand. |

Known architecture facts: the device is rail-mounted, has vertical position `z(t)` and angular orientation `theta(t)` as primary controlled variables, and conceptually has two propulsion channels. These facts do not establish numerical mass, support force, friction, geometry, or operating limits.

## System-level capability requirements

The assembled system must provide enough net vertical force for equilibrium and commanded acceleration throughout the defined operating envelope; sufficient differential torque to achieve the required angular response; sensing coverage and freshness adequate for the eventual control design; and local inhibition/stop behavior that does not depend on the dashboard. Numeric limits must be derived from measured mechanics, disturbance and performance targets, then verified against component data and bench tests.

## Hardware requirements matrix

| Subsystem | Required capability | Design input needed | Status | Selection dependency | Verification method |
|---|---|---|---|---|---|
| Structure/carriage | Carry complete moving assembly throughout travel without unsafe interference | Mass, envelope, rail loads, stops | TO MEASURE / TBD | All moving hardware | Inspection, weighing, travel test |
| Propulsion pair | Deliver required total and differential thrust over operating voltage and motion envelope | Mass, support force, acceleration, inertia, moment arm, disturbances | TO CALCULATE | Motor/prop/ESC/battery system | Thrust stand, current logging, controlled integration test |
| Motors | Meet thrust, response, voltage, current, geometry, and mass needs | Propulsion model and installation envelope | TBD | Propeller, ESC, battery | Manufacturer data plus test stand |
| Propellers | Deliver thrust/efficiency within current, clearance, and safety constraints | Thrust demand, motor, voltage, clearance | TBD | Motor, ESC, voltage, guards | Thrust/current test and clearance inspection |
| ESCs | Drive chosen motors predictably and enter defined safe state on loss/fault | Motor/prop data, protocol, safety behavior | TBD | Motor and controller interface | Bench command, startup, timeout, thermal tests |
| Battery | Supply bus voltage/current for required duration and peaks | Measured load profile, runtime target, mass allowance | TBD | Propulsion and regulator | Load test, voltage sag and thermal measurement |
| Logic supply | Power controller/sensors through steady and transient loads with acceptable noise | Device rails, maximum simultaneous current, battery range | TBD | All logic modules | Load-step, ripple/noise, reset and sensor-validity test |
| IMU | Provide orientation and angular-rate data with suitable freshness and mounting | Required rates, latency, frame, vibration | KNOWN assumption: BNO085 likely; verify | Control bandwidth and breakout | Datasheet review and motion/vibration test |
| ToF | Measure height across usable range with suitable rate and target tolerance | Travel, surface, ambient light, geometry | TBD | Height control and mounting | Range/rate test over travel and lighting |
| Limit switches | Detect travel boundaries with defined fault behavior | Stops, mounting, fail-safe decision | TBD | Safety state behavior | Actuation, open/short and debounce tests |
| Gamepad | Deliver mapped operator commands and detectable disconnect state | Controller generation, command mapping, timeout | TBD | Input policy | Pairing, update/disconnect test |
| Physical interruption | Interrupt hazardous propulsion energy through a defined path | Current, voltage, DC duty, safe restart policy | TBD | Battery and power path | Inspection and appropriately controlled interruption test |

## Selection gate

Component selection can begin only after the applicable mechanical measurements and performance targets are available. Part screening against provisional ranges may be useful, but must remain explicitly provisional. Final selection requires documented requirements, comparable manufacturer/test data, interface compatibility, and validation evidence. RAIL-F02 does not change the RAIL-F01 system architecture.
