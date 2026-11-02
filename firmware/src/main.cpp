#include <Arduino.h>

#include "config/FirmwareConfig.hpp"
#include "core/StateMachine.hpp"
#include "core/SystemTypes.hpp"
#include "safety/SafetyManager.hpp"
#include "telemetry/TelemetryBuilder.hpp"

namespace {

raildrone::StateMachine state_machine;
raildrone::SafetyManager safety_manager;
std::uint32_t last_heartbeat_ms = 0;

}  // namespace

void setup() {
    Serial.begin(raildrone::config::kSerialBaudRate);
    delay(1000);

    Serial.println();
    Serial.println("==============================");
    Serial.println(" RailDrone Control System");
    Serial.println(" Firmware architecture skeleton ready");
    Serial.println("==============================");

    // No sensor readings or actuator commands are fabricated here.
    state_machine.begin_self_test();
}

void loop() {
    raildrone::SystemState state{};
    state.timestamp_ms = millis();
    const raildrone::OperatorCommand reference{};
    const raildrone::ControlEffort effort{};
    const raildrone::MotorCommands mixer_output{};
    const raildrone::SafetyRequirements requirements{};
    const auto safe_output = safety_manager.enforce(
        state, state_machine.mode(), requirements, reference, effort, mixer_output);
    const auto telemetry = raildrone::make_telemetry(
        state, state_machine.mode(), raildrone::ControlMode::MANUAL,
        reference, safe_output.motor_commands, state_machine.fault());

    if (telemetry.timestamp_ms - last_heartbeat_ms >=
        raildrone::config::kHeartbeatIntervalMs) {
        Serial.println("RailDrone alive — hardware unavailable; outputs inhibited");
        last_heartbeat_ms = telemetry.timestamp_ms;
    }
    delay(10);
}
