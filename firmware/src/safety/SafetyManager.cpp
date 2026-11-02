#include "safety/SafetyManager.hpp"

#include <cmath>

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

bool command_is_finite(const OperatorCommand& command) noexcept {
    return std::isfinite(command.vertical_command) &&
           std::isfinite(command.angular_command) &&
           (!command.altitude_reference_valid || std::isfinite(command.altitude_reference_m)) &&
           (!command.angle_reference_valid || std::isfinite(command.angle_reference_rad));
}

}  // namespace

AuthorizedMotorCommands::AuthorizedMotorCommands(
    const StateMachine& state_machine, const SafetyManager& safety_manager,
    float motor_1, float motor_2, bool authorized) noexcept
    : motor_1_(motor_1), motor_2_(motor_2), state_machine_(&state_machine),
      safety_manager_(&safety_manager),
      state_generation_(state_machine.transition_generation()),
      safety_generation_(safety_manager.authorization_generation()),
      authorized_(authorized) {}

AuthorizedMotorCommands::AuthorizedMotorCommands(AuthorizedMotorCommands&& other) noexcept
    : motor_1_(other.motor_1_), motor_2_(other.motor_2_),
      state_machine_(other.state_machine_), safety_manager_(other.safety_manager_),
      state_generation_(other.state_generation_), safety_generation_(other.safety_generation_),
      authorized_(other.authorized_) {
    other.authorized_ = false;
}

AuthorizedMotorCommands& AuthorizedMotorCommands::operator=(
    AuthorizedMotorCommands&& other) noexcept {
    if (this != &other) {
        motor_1_ = other.motor_1_;
        motor_2_ = other.motor_2_;
        state_machine_ = other.state_machine_;
        safety_manager_ = other.safety_manager_;
        state_generation_ = other.state_generation_;
        safety_generation_ = other.safety_generation_;
        authorized_ = other.authorized_;
        other.authorized_ = false;
    }
    return *this;
}

SafetyRequirements SafetyRequirements::configured(bool imu_required, bool tof_required,
                                                   bool limit_switches_required,
                                                   bool operator_input_required) noexcept {
    SafetyRequirements result;
    result.configured_ = true;
    result.imu_required_ = imu_required;
    result.tof_required_ = tof_required;
    result.limit_switches_required_ = limit_switches_required;
    result.operator_input_required_ = operator_input_required;
    return result;
}

SafetyResult SafetyManager::enforce(const StateMachine& state_machine, const SystemState& state,
                                    const SafetyRequirements& requirements,
                                    const OperatorCommand& command,
                                    const MotorMixResult& mix) noexcept {
    ++authorization_generation_;
    const auto inhibited = [this, &state_machine](FaultCode fault,
                                                   bool upper = false, bool lower = false) {
        return SafetyResult(AuthorizedMotorCommands(state_machine, *this, 0.0F, 0.0F, false),
                            fault, true, upper, lower);
    };

    if (state_machine.mode() == SystemMode::EMERGENCY_STOP ||
        command.emergency_stop_requested) {
        return inhibited(FaultCode::EMERGENCY_STOP);
    }
    if (state_machine.mode() == SystemMode::FAULT) {
        return inhibited(state_machine.fault());
    }
    if (mix.fault() != FaultCode::NONE || !mix.valid() || !command_is_finite(command)) {
        return inhibited(FaultCode::INVALID_CONTROL_COMMAND);
    }
    if (!std::isfinite(mix.effort().altitude_effort) ||
        !std::isfinite(mix.effort().angle_effort) ||
        !std::isfinite(mix.commands().motor_1) || !std::isfinite(mix.commands().motor_2)) {
        return inhibited(FaultCode::INVALID_CONTROL_COMMAND);
    }
    if (state_machine.mode() != SystemMode::RUNNING) {
        return inhibited(FaultCode::NONE);
    }
    if (!requirements.is_configured()) {
        return inhibited(FaultCode::SAFETY_POLICY_UNCONFIGURED);
    }
    if (requirements.imu_required() && !state.imu_ok) {
        return inhibited(FaultCode::IMU_UNAVAILABLE);
    }
    if (requirements.tof_required() && !state.tof_ok) {
        return inhibited(FaultCode::TOF_UNAVAILABLE);
    }
    if (requirements.limit_switches_required() &&
        (!state.upper_limit_valid || !state.lower_limit_valid)) {
        return inhibited(FaultCode::LIMIT_SWITCHES_UNAVAILABLE);
    }
    if (requirements.operator_input_required()) {
        if (!state.gamepad_connected) {
            return inhibited(FaultCode::GAMEPAD_LOST);
        }
        if (!state.gamepad_fresh) {
            return inhibited(FaultCode::GAMEPAD_STALE);
        }
    }

    // A normal travel-limit block is an inhibition, not a system fault. With
    // thrust mapping TBD, inhibit both normalized outputs for an outward effort.
    if (state.upper_limit && mix.effort().altitude_effort > 0.0F) {
        return inhibited(FaultCode::NONE, true, false);
    }
    if (state.lower_limit && mix.effort().altitude_effort < 0.0F) {
        return inhibited(FaultCode::NONE, false, true);
    }

    return SafetyResult(
        AuthorizedMotorCommands(state_machine, *this,
                                clamp_motor_command(mix.commands().motor_1),
                                clamp_motor_command(mix.commands().motor_2), true),
        FaultCode::NONE, false, false, false);
}

}  // namespace raildrone
