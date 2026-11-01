# RAIL-F02M — Physical Measurement Campaign

**Project:** RailDrone Control System
**Purpose:** Define repeatable physical measurements and decisions needed before final propulsion, power, and ToF component selection.
**Status:** Planning document; no campaign values or performance targets are asserted here.

## Boundary and recording rules

Use SI units throughout. Record the date, operator, physical configuration, included/excluded items, measurement location, instrument and calibration status, method, number of repeats, uncertainty or instrument resolution, and evidence (photo, sketch, or raw readings). Keep raw readings; do not report more precision than the instrument supports. Mark unobserved items `NOT MEASURED`; label calculations `ESTIMATED` and retain their inputs and method. A decision field remains `TEAM DECISION REQUIRED` until approved and attributed to the team/source.

Do not select motors, ESCs, propellers, batteries, or ToF hardware as part of this campaign. Do not run powered-propeller tests under this task. Maintain the existing RAIL-F01 architecture and RAIL-F02 requirements as the governing baseline.

## Coordinate convention

Establish and photograph a fixed right-handed frame on the carriage before recording geometry:

- Origin `O`: a durable, documented datum on the carriage near the controlled rotation axis.
- `+z`: upward along the rail; rail travel is measured along this axis.
- `+x`: along the line joining the two nominal motor thrust-axis locations, positive toward the designated motor 1 side.
- `+y`: completes the right-handed frame. Record the positive angular rotation axis and the positive `theta` direction explicitly.

If the actual mechanism axes differ, draw the actual frame and state the transform. Record motor thrust-axis positions as coordinates `r_i = (x_i, y_i, z_i)` from `O`, plus thrust direction unit vectors. Do not substitute motor-center spacing for a perpendicular moment arm. For an axis unit vector `e_theta`, the signed moment arm of a parallel force line may be derived from `e_theta · (r_i × e_Ti)`; preserve signs and use the full vector relation for nonparallel axes.

## Campaign procedures

### A. Mechanical geometry and moving envelope

1. With propulsion unpowered and the assembly safely supported, identify the rail axis, rotation axis, datums, mechanical stops, and any cable or guard constraints. Sketch and photograph the coordinate frame.
2. Measure each motor thrust-axis position and direction relative to the datum. Measure motor-to-motor separation between thrust axes. State whether values are centerline-to-centerline and include instrument resolution.
3. Calculate each perpendicular effective moment arm about the controlled rotational axis from the measured geometry. Retain the coordinate inputs and calculation; do not assume symmetry.
4. Measure maximum available propeller diameter as an envelope constraint: identify the minimum clearance around the full swept disk across all allowed carriage positions and angles. Include rail, carriage, structure, guards, cables, stops, and access constraints. This is a geometric limit, not a propeller selection.
5. Measure usable vertical travel between lower and upper mechanical limits. Record endpoint locations, restricted zones, stop type, repeatability, and any unavailable deceleration distance. Measure permitted angular travel to each side of its reference and document interference, wire twist/drag, and stop constraints.
6. Repeat clearance observations at the limiting combinations of vertical and angular positions. Record the limiting obstacle and the method used to infer any proposed maximum diameter.

### B. Mass

Use a suitable scale on a stable surface. Record scale resolution, calibration/check method, tare, repeat readings, and uncertainty. Prevent support, cables, or fixtures from carrying unrecorded load. Define boundaries to avoid double-counting.

- **Carriage mass:** weigh the carriage and every item that translates with it, with an inclusion list.
- **Propulsion assembly mass:** weigh propulsion-related moving items separately where practical (motor, mount, propeller, ESC if carried, and moving wiring); document what is included.
- **Electronics mass:** weigh controller, sensors, wiring, and other electronics with the same boundary discipline.
- **Battery mass:** measure when a candidate/representative battery is available; until then leave unmeasured and do not invent a substitute.
- **Complete moving mass:** weigh the assembled representative moving configuration, including all carried structure, wiring, guards, payload, and battery as applicable. Record configuration and reconcile against the sum of separately measured masses.
- **Estimated final system mass:** calculate from measured subassembly masses plus explicit estimates for unbuilt items, fasteners, wiring, and other omissions. Mark `ESTIMATED`, show the itemized basis and uncertainty/range, and update against the completed assembly mass.

### C. Center of mass

Use the established carriage frame. For a compact assembly, locate the center of mass with a repeatable balance method on two independent support orientations/axes, keeping the assembly stable and safely supported. For irregular or rail-constrained assemblies, use calibrated reaction/load measurements at known support points or a documented suspension/plumb-line method; solve for the intersection/weighted balance location. Do not rely on a single balance line to claim a 3D location.

Measure the complete representative moving assembly and state included configuration. Repeat each balance/reaction measurement, include fixture and datum uncertainty, and report coordinate-wise uncertainty or bounds. Repeat when battery or payload position changes materially. The center of mass matters because an offset from the controlled axis creates a gravity torque that changes with angle, and because thrust moment arms and angular/vertical coupling depend on the mass center.

### D. Rail force, support reaction, and resistance

The rail may carry some or all of the weight; separate this effect from friction/resistance. Use a calibrated force gauge or load cell aligned with the rail, attached to the representative moving assembly through a safe, low-stretch connection. A calibrated hanging-mass method may be used only with a known pulley/friction arrangement and secure catch; report its added friction and geometry. No force values are presumed.

1. Record assembly mass/configuration, rail orientation, cable routing, and carriage location. If the rail is not vertical, determine the gravity component along the travel axis from measured orientation; keep it as a signed gravity contribution, not rail friction.
2. At several documented positions across the usable travel, increase applied force slowly until motion begins. Record peak breakaway force in both travel directions. Repeat enough times to characterize repeatability and report the range and instrument uncertainty.
3. Pull/push at a controlled, documented slow speed in each direction and record approximate running force at the same positions. Where velocity cannot be controlled, record actual observation and treat the result as an estimate.
4. Repeat with representative moving mass and realistic cable routing. Note binding, stick-slip, rail joints, misalignment, contamination, preload, and changes with position/direction. Inspect before attributing a high reading to friction.
5. If propulsion force must also support weight, measure or derive vertical rail reaction separately using calibrated supports/load cells or a documented force balance. Do not infer support reaction from breakaway force alone. State sign convention and include uncertainty.

Report a position- and direction-dependent characterization, not one unexplained scalar. The propulsion force model must include gravity, rail reaction, resistance, and credible disturbances with consistent signs. Repeat after mechanical adjustments.

### E. ToF environment observations

At the proposed sensor-to-target path, measure minimum and maximum distance across the usable travel and angular envelope, including tolerances and occlusions. Record target material, color, finish/reflectivity, size, angle, background, and whether the spot can leave the target. Observe expected ambient lighting, including bright/dark conditions, sunlight or artificial sources, flicker, and shadows; note when and where observations were made.

Record mounting orientation, available optical path, field-of-view restrictions from structure/guards, and vibration/motion at the mount. The team must define required update rate and distance resolution in the performance decision process. No ToF model is selected by these observations.

### F. Performance targets and disturbance expectations

The team must agree on desired vertical acceleration (m/s²), maximum vertical speed (m/s), desired angular acceleration (rad/s²), maximum angular speed (rad/s), settling behavior (define start condition, tolerance band, and time), maximum permitted overshoot (define reference and units), disturbance rejection expectation (define disturbance cases and acceptable recovery), and desired runtime (s or min with duty cycle/operating profile). Mark every unapproved target `TEAM DECISION REQUIRED`; include approver/date/rationale when decided. These are design inputs, not measured facts.

### G. Moment of inertia

Do not assign an inertia value until the complete mass distribution and axis are documented. Required inputs are measured component masses, each component center location in the carriage frame, component dimensions/shape and orientation, moving cables/guards/payload/battery configuration, and the controlled rotational axis. Estimate by summing component centroidal inertias plus parallel-axis terms, or approximate the assembly with documented simple shapes where dimensions and mass allocation justify it. Report assumptions and uncertainty.

CAD mass properties may be used when the model represents the measured final moving configuration, material densities/mass properties are verified against measured masses, fasteners/wiring/guards/battery/payload are represented or bounded, and the selected axis matches the physical controlled axis. Compare CAD total mass to scale measurements. Prefer later experimental identification (for example, a safely constrained torsional or physical-pendulum test with known geometry and damping treatment) when distribution is irregular, CAD is incomplete, uncertainty dominates, or measured angular response disagrees with the model. Document the method and safety review before such a test.

## Propulsion model input table

Fill from measured data and approved targets before RAIL-F03 computes demands. Use signed force/torque conventions and retain dependencies; no values are supplied here.

| Model quantity | Required inputs / relation | Dependencies |
|---|---|---|
| Static force requirement | Force balance at rest: weight, signed rail vertical reaction/support, resistance or preload, cable forces, and defined disturbances/configuration. | Complete moving mass; rail orientation; support/reaction and resistance characterization; disturbance cases. |
| Thrust margin | Agreed margin above worst-case modeled force and uncertainty; rationale includes operating envelope and control authority. | Static and dynamic force envelope; uncertainty; disturbance expectation; team target/decision. |
| Individual motor thrust requirement | Feasible channel thrust allocation for total-force and torque demands; evaluate limits and asymmetric geometry rather than assuming equal split. | Total force requirement; motor thrust-axis geometry/direction; effective arms; required torque; candidate thrust curves later. |
| Differential thrust requirement | Channel thrust difference/range needed for requested signed torque, including unequal lever arms if applicable. | Controlled axis; thrust vectors/positions; moment arms; angular torque requirement; channel operating points. |
| Angular torque requirement | `tau_net = I * alpha` plus gravity torque from center-of-mass offset, rail/mechanism effects, and defined disturbances. | Inertia estimate/measurement; center of mass; angular acceleration target; permitted angular envelope; resistance/disturbance characterization. |

For vertical motion, use a signed force balance such as `m*a_z = T_z + F_rail,z - m*g + F_disturbance,z`, adapting it to measured load paths. For general angular geometry use `tau = sum(r_i × F_i)` about the documented axis. Equations do not provide numeric answers without inputs.

## Recommended measurement order

1. **Mechanical geometry:** establish datums/axes, motor thrust lines, stops, rail orientation, angular/vertical constraints, and envelope.
2. **Mass:** weigh subassemblies, then the representative complete moving assembly; reconcile boundaries.
3. **Center of mass:** locate in the same frame/configuration as the complete mass.
4. **Rail characterization:** measure breakaway/running force and vertical support reaction with representative load and routing.
5. **Travel and clearances:** verify travel endpoints, angle, swept-disk envelope, and limiting conditions over full motion.
6. **Performance decisions:** agree preliminary performance, disturbance, and runtime targets; record ownership and rationale.
7. **Derived parameters:** calculate moment arms and preliminary inertia, with traceable inputs and uncertainty.
8. **Propulsion sizing:** only after the readiness gate below; evaluate candidates with evidence and the RAIL-F03 process.

Some geometry and travel observations can be collected in one fixture session; retain the dependency order for derived values.

## RAIL-F03 go/no-go gate

RAIL-F03 is **not ready** to begin final propulsion sizing unless all of the following are recorded and reviewed:

- representative complete moving mass and configuration;
- rail resistance/breakaway/running and vertical reaction characterization across travel, direction, and representative load;
- motor thrust-axis geometry and derived effective moment arm;
- propeller swept-disk clearance and maximum available diameter constraint;
- usable vertical travel and upper/lower mechanical limits;
- permitted angular travel or an approved required angular envelope;
- center-of-mass estimate with coordinate frame and uncertainty;
- agreed preliminary vertical/angular performance, disturbance, and runtime targets.

Missing or weak evidence is a no-go for final selection; record the gap and its consequence. Early desk research may identify data needs but does not close this gate. RAIL-F03 also needs a traceable inertia estimate or an explicit plan to obtain one before angular torque is finalized.

## Campaign completion record

Use [the measurement data sheet](measurement-data-sheet.md) as the controlled blank register. Attach raw observations and calculation records; date/revision the filled copy. Update the register rather than this procedure when measurements become available.
