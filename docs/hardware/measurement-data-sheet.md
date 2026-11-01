# RAIL-F02M — Measurement Data Sheet

Blank register for the physical campaign. Enter one value per row where practical; use additional rows for positions, directions, configurations, or repeated observations. Keep raw readings and evidence with the campaign record. Do not invent values. Use SI units and record uncertainty or instrument resolution.

**Campaign date:** TEAM DECISION REQUIRED
**Operator(s):** TEAM DECISION REQUIRED
**Assembly/configuration ID:** NOT MEASURED
**Coordinate-frame sketch/photo reference:** NOT MEASURED

## Measurement register

| Parameter | Symbol | Value | Unit | Measurement method | Instrument | Uncertainty / resolution | Status | Notes |
|---|---|---|---|---|---|---|---|---|
| Carriage mass | `m_carriage` |  | kg | Weigh defined carriage assembly |  |  | NOT MEASURED | List included items |
| Propulsion assembly mass | `m_propulsion` |  | kg | Weigh defined propulsion subassembly |  |  | NOT MEASURED | No hardware selected yet |
| Electronics mass | `m_electronics` |  | kg | Weigh defined electronics subassembly |  |  | NOT MEASURED | List wiring/sensors included |
| Battery mass | `m_battery` |  | kg | Weigh representative battery when available |  |  | NOT MEASURED | Candidate unavailable until evaluated |
| Complete moving mass | `m_moving` |  | kg | Weigh assembled representative configuration |  |  | NOT MEASURED | Include payload, guards, wiring, battery as applicable |
| Estimated final system mass | `m_final,est` |  | kg | Itemized sum plus explicit estimates |  |  | ESTIMATED | Keep estimate distinct from measurement |
| Rail axis orientation | `e_z` |  | unit vector / deg | Survey relative to frame/gravity |  |  | NOT MEASURED | State signed travel direction |
| Controlled rotation axis | `e_theta` |  | unit vector | Inspect/survey mechanism |  |  | NOT MEASURED | Define positive angular direction |
| Motor 1 thrust-axis position | `r_1` |  | m (x,y,z) | Survey from documented datum |  |  | NOT MEASURED | Coordinates in carriage frame |
| Motor 2 thrust-axis position | `r_2` |  | m (x,y,z) | Survey from documented datum |  |  | NOT MEASURED | Coordinates in carriage frame |
| Motor 1 thrust direction | `e_T1` |  | unit vector | Survey/verify intended thrust direction |  |  | NOT MEASURED | Express in carriage frame |
| Motor 2 thrust direction | `e_T2` |  | unit vector | Survey/verify intended thrust direction |  |  | NOT MEASURED | Express in carriage frame |
| Motor thrust-axis separation | `d_motor` |  | m | Measure centerline separation |  |  | NOT MEASURED | Specify endpoints/direction |
| Effective moment arm, channel 1 | `r_eff,1` |  | m | Calculate from axis and thrust line |  |  | ESTIMATED | Preserve signed geometry inputs |
| Effective moment arm, channel 2 | `r_eff,2` |  | m | Calculate from axis and thrust line |  |  | ESTIMATED | Preserve signed geometry inputs |
| Center-of-mass coordinates | `r_CM` |  | m (x,y,z) | Balance/reaction method in defined frame |  |  | NOT MEASURED | State assembly configuration |
| Maximum available propeller diameter | `D_max` |  | m | Swept-disk clearance envelope survey |  |  | NOT MEASURED | Geometric limit only |
| Minimum propeller clearance | `c_prop,min` |  | m | Check full swept disk and motion envelope |  |  | NOT MEASURED | Identify obstacle and position |
| Usable vertical travel | `z_travel` |  | m | Measure between restricted limits |  |  | NOT MEASURED | Record endpoint coordinates |
| Upper mechanical limit | `z_upper` |  | m | Locate stop/limit from datum |  |  | NOT MEASURED | Describe stop and restricted zone |
| Lower mechanical limit | `z_lower` |  | m | Locate stop/limit from datum |  |  | NOT MEASURED | Describe stop and restricted zone |
| Permitted angular travel | `theta_min`, `theta_max` |  | rad (deg optional) | Sweep unpowered mechanism and inspect |  |  | NOT MEASURED | Include wiring/interference limits |
| Rail breakaway force by position/direction | `F_breakaway(z,dir)` |  | N | Force gauge/load cell, repeated pull/push |  |  | NOT MEASURED | Separate gravity contribution |
| Approximate running force by position/direction | `F_run(z,dir)` |  | N | Controlled slow pull/push |  |  | NOT MEASURED | Record speed and stick-slip |
| Vertical rail support/reaction | `F_rail,z` |  | N | Calibrated reaction/load measurement or force balance |  |  | NOT MEASURED | Breakaway force alone is insufficient |
| Rail binding/variation observations | `—` |  | — | Inspect and repeat at positions |  |  | NOT MEASURED | Note joints, contamination, preload |
| ToF minimum path distance | `d_ToF,min` |  | m | Survey optical path over envelope |  |  | NOT MEASURED | Include angle and tolerance |
| ToF maximum path distance | `d_ToF,max` |  | m | Survey optical path over envelope |  |  | NOT MEASURED | Include angle and tolerance |
| ToF target material/color/finish | `—` |  | descriptive | Observe actual target |  |  | NOT MEASURED | Note reflectivity, size, angle, background |
| ToF ambient-light conditions | `—` |  | descriptive / lx if measured | Observe expected environment |  |  | NOT MEASURED | Record time/place/light source |
| ToF mount orientation/FOV restrictions | `—` |  | deg / descriptive | Inspect optical path and occlusions |  |  | NOT MEASURED | Include vibration/motion observations |
| Moment of inertia about controlled axis | `I_theta` |  | kg·m² | Calculate from mass distribution or later test |  |  | NOT MEASURED | Mark ESTIMATED only after a documented estimate exists |

## Team performance and design decisions

Each row remains `TEAM DECISION REQUIRED` until the team approves a value/definition, owner, date, and rationale.

| Decision | Symbol | Agreed value / definition | Unit | Status | Owner/date/rationale |
|---|---|---|---|---|---|
| Desired vertical acceleration | `a_z,desired` |  | m/s² | TEAM DECISION REQUIRED |  |
| Maximum vertical speed | `v_z,max` |  | m/s | TEAM DECISION REQUIRED |  |
| Desired angular acceleration | `alpha_theta,desired` |  | rad/s² | TEAM DECISION REQUIRED |  |
| Maximum angular speed | `omega_theta,max` |  | rad/s | TEAM DECISION REQUIRED |  |
| Settling behavior | `t_settle`, tolerance | Define initial condition, tolerance band, and allowed time | s and defined tolerance | TEAM DECISION REQUIRED |  |
| Maximum permitted overshoot | `OS_max` | Define reference and whether absolute or percentage | %, rad, or m as applicable | TEAM DECISION REQUIRED |  |
| Disturbance rejection expectation | `D_reject` | Define disturbance cases and acceptable recovery | Defined force/torque and response | TEAM DECISION REQUIRED |  |
| Desired runtime | `t_runtime` | Define operating profile/duty cycle | min or s | TEAM DECISION REQUIRED |  |
| Required ToF update rate | `f_ToF` |  | Hz | TEAM DECISION REQUIRED |  |
| Required ToF distance resolution | `res_ToF` |  | m or mm | TEAM DECISION REQUIRED |  |

## Power and energy inputs for later battery sizing

Motor current remains unavailable until propulsion candidates and their operating points are evaluated. Do not fill it with a guessed value.

| Input | Symbol | Value | Unit | Source/method | Status | Notes |
|---|---|---|---|---|---|---|
| Estimated motor current by operating point | `I_motor` |  | A | Candidate motor/propulsion data and later test | NOT MEASURED | Not available before candidates |
| Expected average propulsion demand | `P_prop,avg` |  | W | Duty-cycle/load model and validation | NOT MEASURED | State profile |
| Expected peak propulsion demand | `P_prop,peak` |  | W | Worst-case operating envelope and transient | NOT MEASURED | State duration |
| Electronics current | `I_electronics` |  | A | Sum verified rail loads or measure | NOT MEASURED | Include startup/transients |
| Desired runtime | `t_runtime` |  | min or s | Team-approved performance decision | TEAM DECISION REQUIRED | Same profile as demand estimate |
| Acceptable battery mass | `m_battery,max` |  | kg | Team/mechanical decision | TEAM DECISION REQUIRED | Recheck moving mass and inertia |
| Available physical envelope | `V_battery` |  | m (dimensions) | Measure mounting/clearance volume | NOT MEASURED | Include retention, connector access, service |

## Propulsion model input checklist for RAIL-F03

| Required output/input | Necessary measured/decided dependencies | Status |
|---|---|---|
| Static force requirement | Representative moving mass; gravity projection; rail support reaction and resistance vs position/direction; cable load and disturbance cases | NOT MEASURED |
| Thrust margin and rationale | Worst-case force envelope; uncertainty; disturbance expectation; performance targets and control-authority rationale | TEAM DECISION REQUIRED |
| Individual motor thrust requirement | Static/dynamic total force; thrust-axis geometry; effective arms; torque demand; candidate motor/prop thrust data later | NOT MEASURED |
| Differential thrust requirement | Desired signed torque; motor positions and thrust vectors; effective arms; channel operating point | NOT MEASURED |
| Angular torque requirement | Inertia; center of mass; angular acceleration target; gravity offset; angular travel; mechanism resistance and disturbance cases | NOT MEASURED |

## Readiness gate

| Gate item | Evidence reference | Status | Gap / owner |
|---|---|---|---|
| Representative moving mass |  | NOT MEASURED |  |
| Rail resistance and reaction characterization |  | NOT MEASURED |  |
| Motor geometry/effective moment arm |  | NOT MEASURED |  |
| Propeller clearance/maximum diameter constraint |  | NOT MEASURED |  |
| Vertical travel and upper/lower limits |  | NOT MEASURED |  |
| Angular travel or required angular envelope |  | NOT MEASURED |  |
| Center-of-mass estimate with uncertainty |  | NOT MEASURED |  |
| Preliminary performance targets approved |  | TEAM DECISION REQUIRED |  |
| Preliminary inertia estimate or plan to identify it |  | NOT MEASURED |  |
