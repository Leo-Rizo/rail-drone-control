#include "safety/SafetyManager.hpp"

#include "config/FirmwareConfig.hpp"

namespace raildrone {
namespace {

float clamp_motor_command(float value) noexcept {
    if (value < config::kMinimumMotorCommand) {
        return config::kMinimumMotorCommand;
    }
    if (value > config::kMaximumMotorCommand) {
        return config::kMaximumMotorCommand;
    }
    return value;
}

}  // namespace

SafetyResult SafetyManager::enforce(const SystemState& state, SystemMode mode,
                                    const SafetyRequirements& requirements,
                                    const OperatorCommand& command,
                                    const ControlEffort& effort,
                                    const MotorCommands& requested) const noexcept {
    SafetyResult result{};

    if (mode == SystemMode::EMERGENCY_STOP || command.emergency_stop_requested) {
        result.fault = FaultCode::EMERGENCY_STOP;
        return result;
    }
    if (mode != SystemMode::RUNNING) {
        return result;
    }
    if (requirements.imu_required && !state.imu_ok) {
        result.fault = FaultCode::IMU_UNAVAILABLE;
        return result;
    }
    if (requirements.tof_required && !state.tof_ok) {
        result.fault = FaultCode::TOF_UNAVAILABLE;
        return result;
    }
    if (requirements.limit_switches_required &&
        (!state.upper_limit_valid || !state.lower_limit_valid)) {
        result.fault = FaultCode::LIMIT_SWITCHES_UNAVAILABLE;
        return result;
    }
    if (requirements.operator_input_required && !state.gamepad_connected) {
        result.fault = FaultCode::GAMEPAD_LOST;
        return result;
    }

    // Without a selected thrust mapping, conservatively inhibit both channels
    // when an outward vertical request is detected. Inward escape remains possible.
    if (state.upper_limit && effort.altitude_effort > 0.0F) {
        result.upper_limit_blocked = true;
        return result;
    }
    if (state.lower_limit && effort.altitude_effort < 0.0F) {
        result.lower_limit_blocked = true;
        return result;
    }

    result.motor_commands.motor_1 = clamp_motor_command(requested.motor_1);
    result.motor_commands.motor_2 = clamp_motor_command(requested.motor_2);
    result.inhibited = false;
    return result;
}

}  // namespace raildrone
