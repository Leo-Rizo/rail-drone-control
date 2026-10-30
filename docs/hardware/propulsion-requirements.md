# RAIL-F02 — Propulsion Requirements

## Thrust model and inputs

Use SI units throughout: mass `m` in kg, acceleration `a` in m/s², force/thrust `T` in N, distance/moment arm `r` in m, inertia `I` in kg·m², angular acceleration `alpha` in rad/s², and torque `tau` in N·m. Use local gravitational acceleration `g` in m/s² (approximately 9.81 m/s² near Earth's surface for preliminary calculation; use the applicable value if precision requires it).

For a freely supported moving assembly at static vertical equilibrium:

```text
T_total >= m * g
```

This is an idealized lower bound, not a component rating. If the rail/carriage mechanically supports weight or applies vertical reactions, replace the free-body assumption with a measured/modelled force balance. Define the sign and load path; quantify support force, friction, cable drag, and any preload over the travel and direction. In a simple upward-positive model:

```text
m * a_z = T_total + F_rail - m * g + F_disturbance
```

where `F_rail` is the signed vertical force exerted by the rail/carriage and `F_disturbance` is the signed external force. Do not count an unknown rail reaction as free lift capacity.

For two channels, with calibrated thrust axes and a common positive vertical direction:

```text
T1 + T2 = T_total
```

At symmetric static equilibrium only, an equal split would be `T1 = T2 = T_total / 2`; actual allocation depends on geometry, thrust mismatch, torque demand, and saturation. Required design thrust must exceed equilibrium thrust by an engineering margin. The margin is not selected in RAIL-F02: derive it from uncertainty, disturbances, desired acceleration/control authority, voltage sag, thermal limits, and validation results.

### Provisional requirement framework

- **Hover/equilibrium thrust:** calculate the total thrust needed to balance weight and measured/modelled rail forces for each relevant position/configuration. This may not be conventional free-flight hover.
- **Upward acceleration:** add the net force needed for target upward acceleration, using `sum(F) = m * a`; include signed gravity, rail forces, and disturbances.
- **Control authority:** preserve enough independent thrust range in both channels to command vertical force and differential torque without routine saturation. Derive feasible combinations after motor curves and geometry are known.
- **Disturbance rejection:** establish disturbance cases and recovery requirements, then determine transient thrust and response needs. Inputs are TBD until defined/tested.
- **Thrust margin:** state the margin above the worst-case modeled demand and its rationale; include model uncertainty and operating voltage/thermal conditions. Value TBD.
- **Individual motor thrust:** each channel must meet its assigned maximum and minimum operating thrust over the required envelope; do not assume half the total at all times. Confirm matched response or characterize compensation.

The required inputs include final moving mass, rail vertical reaction and friction, desired vertical acceleration and speed, allowed travel, external disturbances, motor thrust curves versus voltage/propeller, current draw, thermal limits, and channel dynamics. No final thrust value is supportable before these are established.

## Angular control and differential thrust

For two approximately parallel, oppositely placed thrust lines about the controlled axis, the conceptual relationship is:

```text
tau = (T1 - T2) * effective_moment_arm
```

This simplified form assumes equal, opposite perpendicular arms and compatible thrust directions. For general geometry, calculate the vector sum:

```text
tau_vector = sum(r_i cross F_i)
```

Required torque follows rotational dynamics, for a rigid body about a fixed principal axis:

```text
tau_net = I * alpha
```

Account for gravity/center-of-mass offset, rail constraints, friction, angular disturbances, coupling with translation, and motor/propeller response as applicable. Required inputs: moment of inertia, motor spacing and thrust-line geometry/effective arm, desired angular acceleration and response time, allowable angular range, mechanical stops/constraints, angular disturbance cases, and baseline/maximum thrust per channel. Values remain TO MEASURE, TO CALCULATE, or TBD per [system requirements](system-requirements.md).

The final design must demonstrate adequate positive and negative torque authority throughout allowed vertical operation without exceeding motor, ESC, battery, mechanical, or propeller limits. Saturation and failure behavior require later control and safety design; no mixer or firmware is defined here.

## Motor selection criteria — no part selected

- **Type:** BLDC is the expected motor type from the current concept; confirm compatibility with selected ESC and propeller.
- **Thrust:** verified thrust/current curves for the intended propeller and voltage must cover each channel's calculated operating range, margin, and transients.
- **Voltage/current:** operating voltage must cover the battery's loaded range; current must remain within motor, ESC, wiring, connector, and battery limits continuously and during peaks.
- **Propeller range:** manufacturer-supported diameter/pitch and rotation direction must include candidate propellers without exceeding RPM, current, thermal, or mechanical limits.
- **Response:** characterize thrust rise/fall and command response against required vertical/angular dynamics; KV does not describe this adequately.
- **Packaging:** dimensions, shaft, mass, mounting pattern, cooling, wire exit, and propeller clearance must fit the measured envelope.
- **Evidence and procurement:** prefer traceable datasheets and repeatable thrust-stand data at stated voltage, propeller, and test conditions; record availability and replacement consistency.

KV alone is insufficient: it is a speed constant, not a thrust/current/efficiency curve. Motor + propeller + voltage must be evaluated as a system, preferably using manufacturer data and then representative tests.

## Propeller selection criteria — no part selected

Determine diameter, pitch, blade count/material, rotation direction, and mounting from verified motor performance and required thrust. The system evaluation must document thrust, current, RPM, efficiency, temperature, voltage, and test conditions. Check swept-disk clearance over the entire vertical/angular travel, rail and structure clearances, guard feasibility, and operator hazard controls. Propeller integrity, retention, balance, and safe handling are required. Never infer performance from nominal dimensions alone.

## ESC selection criteria — no part selected

Specify continuous current for the expected operating profile with thermal conditions, peak current and duration with justified margin, battery voltage/cell compatibility, motor type/pole/timing compatibility, and a control protocol compatible with ESP32 timing and safety requirements. Exact command protocol remains TBD. Characterize startup/arming, loss-of-command behavior, neutral/stop semantics, restart behavior, braking/regenerative behavior if applicable, and any telemetry usefulness. Do not assume an ESC failsafe or numeric zero command removes hazardous thrust; verify the actual ESC and provide independent physical interruption as designed.

## Propulsion verification evidence

Before final freeze, retain: mass/geometry inputs; signed free-body model including rail reactions; thrust and torque demand envelope with margin rationale; motor-prop-voltage test curves; current/thermal limits; clearance inspection; ESC protocol/failsafe test evidence; and an integrated low-risk validation plan. Selection does not authorize powered operation until safety controls and test conditions are reviewed.
