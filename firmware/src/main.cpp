#include <Arduino.h>

#include "config/FirmwareConfig.hpp"
#include "control/MotorMixer.hpp"
#include "core/ApplicationCoordinator.hpp"
#include "core/SystemTypes.hpp"

namespace {

raildrone::ApplicationCoordinator application;
raildrone::MotorMixer motor_mixer;
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
    application.state_machine().begin_self_test();
}

void loop() {
    raildrone::SystemState state{};
    state.timestamp_ms = millis();
    const raildrone::OperatorCommand reference{};
    const raildrone::ControlEffort effort{};
    const auto mixer_output = motor_mixer.mix(0.0F, effort);
    const raildrone::SafetyRequirements requirements{};
    const auto cycle = application.update(
        state, requirements, reference, mixer_output, raildrone::ControlMode::NONE);

    if (cycle.telemetry.timestamp_ms - last_heartbeat_ms >=
        raildrone::config::kHeartbeatIntervalMs) {
        Serial.println("RailDrone alive — hardware unavailable; outputs inhibited");
        last_heartbeat_ms = cycle.telemetry.timestamp_ms;
    }
    delay(10);
}
