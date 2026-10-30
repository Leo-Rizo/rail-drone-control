# RAIL-F02 — Measurement Plan Before Hardware Selection

Obtain and record these measurements on the physical prototype before final motor, propeller, ESC, battery, regulator, or ToF selection. Record date, configuration, instruments, method, units, uncertainty/repeatability, and photographs or sketches where useful. Do not substitute estimates without labeling them.

| Priority | Measurement / definition | Practical method and record | Enables |
|---|---|---|---|
| 1 | Complete moving mass and configuration | Weigh assembled moving unit; itemize what is installed/included and note scale resolution. Repeat after major additions. | Equilibrium thrust and battery mass budget. |
| 1 | Carriage mass and propulsion assembly mass | Weigh these subassemblies separately where practical; define boundaries to prevent double-counting. | Load distribution and packaging tradeoffs. |
| 1 | Rail vertical support/reaction and resistance | Measure supported load and breakaway/running force with a calibrated force gauge or load cell at multiple travel positions and directions; include cable routing and representative mass. | Determine whether propulsion supports weight; net force model. |
| 1 | Usable vertical travel and endpoints | Measure stroke and restricted zones; document mechanical stops and available deceleration distance. | ToF range, switches, clearance. |
| 1 | Propeller installation envelope / clearance | Measure swept-disk keep-outs over full vertical and angular motion, including rail, carriage, structure, wiring, guards, and operator access. | Maximum feasible propeller geometry and safety review. |
| 1 | Motor spacing and thrust-axis geometry | Measure thrust-axis separation, directions, and offsets from rotation axis in a fixed coordinate frame. | Effective moment arm and torque authority. |
| 1 | Center of mass | Balance/locate center of mass in defined coordinates on complete assembly; repeat for variable battery/payload configurations. | Angular gravity moment and dynamic model. |
| 1 | Angular travel and mechanical constraints | Measure allowed angle, stops, interference, wiring twist/drag, and repeatability. Set allowed range through mechanical review. | Torque and safety envelope. |
| 2 | Expected disturbances | Identify representative cable drag, rail contamination/friction variation, external contact/wind if applicable, and impact scenarios; quantify only with a defined method. | Disturbance rejection and engineering margin. |
| 2 | Mass distribution / moment of inertia | Use measured part masses/locations for calculation or a documented inertia test about controlled axis. | Angular torque for desired acceleration. |
| 2 | Mounting volumes and service envelope | Measure candidate spaces, fastener patterns, cooling/airflow, connector access, and cable routes. | Motor, ESC, regulator, sensor packaging. |
| 2 | Target surface and optical path | Document ToF target distance, material/color/angle/size, background, occlusion, and lighting throughout travel. | ToF range, field of view, ambient-light needs. |
| 2 | Required performance targets | Agree desired vertical/angular range, speed, acceleration, settling/recovery, runtime, duty cycle, and operating environment. These are design targets, not physical measurements; record approval/source. | Turns measurements into component capability requirements. |

## Measurement record template

For each measurement, record: parameter; value and units; configuration; method; instrument and calibration/status; date; operator; repeat count; uncertainty/range; evidence location; and whether the value is measured, estimated, or calculated. Keep source data and calculations together so later hardware selection can be traced to assumptions.

## Exit criteria for RAIL-F03

Before freezing component selections, the team should have a reviewed mass/force model including rail reactions, target acceleration and disturbance cases, measured propulsion geometry and clearance, angular travel and inertia, packaging envelope, ToF surface/range conditions, and performance/runtime targets. The propulsion combination must then be evaluated with traceable thrust/current/voltage/thermal data. Unknowns that remain must be stated explicitly with their effect on selection.
