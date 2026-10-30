# RAIL-F02 — Power Requirements

Battery chemistry, cell count, voltage, capacity, regulator/BEC, connectors, conductors, protection, and physical emergency interruption remain unselected as in the [RAIL-F01 power architecture](power-architecture.md). This document defines sizing inputs only.

## Battery sizing inputs

Size the battery from the actual propulsion and logic load profile and the required operating duration. Required design inputs:

- Chemistry and cell count / nominal and full-charge voltage, chosen only after motor/ESC compatibility and safety review.
- Usable capacity in Ah after discharge limits, aging, temperature, and required reserve are considered.
- Continuous and peak discharge capability at expected temperature and voltage; measured/credible current profile from the motor-propeller combination.
- Expected average current, peak current magnitude and duration, voltage sag, startup and transient load.
- Runtime target and duty cycle, including idle/control loads.
- Battery mass allowance and its effect on moving mass/thrust demand.
- Connector polarity/rating, wiring gauge/length, strain relief, serviceability, and compatibility with protection devices.
- Cell/battery monitoring, low-voltage policy, overcurrent/short-circuit protection, physical retention, and safe charging/storage procedure appropriate to the selected chemistry.

The relation `I_available = Capacity_Ah * C_rating` is a first-pass sizing concept: Ah multiplied by a discharge C rating yields amperes. It is not a guarantee of safe deliverable current; confirm manufacturer limits, temperature, voltage sag, connector/wiring limits, aging, and test evidence. Do not select a battery until propulsion demand, runtime target, allowable mass, and voltage range are established.

## Regulated logic rail

The regulator/BEC must support the ESP32 DevKit, BNO085 breakout, selected ToF module, switches/interfaces, battery sensing, and future logic devices. Determine each device's permitted input voltage, steady current, startup/peak current, and simultaneous load; then define the rail voltage and maximum expected current with a documented transient margin. No regulator component or output voltage is frozen here.

Requirements include:

- Compatible input range across battery full-charge, nominal, and loaded low voltage.
- Correct regulated output rail(s) for the exact board/modules, including tolerance and startup behavior.
- Continuous and transient current capacity with thermal margin in the installed enclosure.
- Acceptable ripple, conducted noise, and recovery during ESC/motor load changes; avoid controller/sensor resets and corrupted measurements.
- Deliberate grounding/reference design and return-current paths; assess whether filtering or isolation is needed.
- Local decoupling and wiring/layout designed from selected component guidance and verified in the final assembly.
- Separate/appropriately routed high-current propulsion paths and sensitive IMU/ToF/ADC wiring.
- Protection against reverse polarity, overcurrent, and relevant transients as required by the eventual circuit design.

Test the logic rail under worst credible simultaneous load and propulsion switching conditions. Verify voltage at the ESP32 and sensors, ripple/transients, device resets, bus errors, and sensor validity. The dashboard is supervisory and must not be needed to preserve the local power/safety behavior.
