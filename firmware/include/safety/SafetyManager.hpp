#pragma once

#include <utility>

#include "control/MotorMixer.hpp"
#include "core/StateMachine.hpp"

namespace raildrone {

class SafetyManager;
class IMotorOutput;

class SafetyRequirements {
public:
    SafetyRequirements() noexcept = default;

    static SafetyRequirements configured(bool imu_required, bool tof_required,
                                         bool limit_switches_required,
                                         bool operator_input_required) noexcept;

    bool is_configured() const noexcept { return configured_; }
    bool imu_required() const noexcept { return imu_required_; }
    bool tof_required() const noexcept { return tof_required_; }
    bool limit_switches_required() const noexcept { return limit_switches_required_; }
    bool operator_input_required() const noexcept { return operator_input_required_; }

private:
    bool configured_{false};
    bool imu_required_{false};
    bool tof_required_{false};
    bool limit_switches_required_{false};
    bool operator_input_required_{false};
};

// Opaque actuator request. Only SafetyManager can create one; implementations
// of IMotorOutput can inspect its normalized values but cannot mint commands.
class AuthorizedMotorCommands {
public:
    AuthorizedMotorCommands(const AuthorizedMotorCommands&) = delete;
    AuthorizedMotorCommands& operator=(const AuthorizedMotorCommands&) = delete;
    AuthorizedMotorCommands(AuthorizedMotorCommands&& other) noexcept;
    AuthorizedMotorCommands& operator=(AuthorizedMotorCommands&& other) noexcept;

    float motor_1() const noexcept { return motor_1_; }
    float motor_2() const noexcept { return motor_2_; }

private:
    AuthorizedMotorCommands(const StateMachine& state_machine, const SafetyManager& safety_manager,
                            float motor_1, float motor_2, bool authorized) noexcept;

    float motor_1_{0.0F};
    float motor_2_{0.0F};
    const StateMachine* state_machine_{nullptr};
    const SafetyManager* safety_manager_{nullptr};
    std::uint32_t state_generation_{0};
    std::uint32_t safety_generation_{0};
    bool authorized_{false};

    friend class SafetyManager;
    friend class IMotorOutput;
};

class SafetyResult {
public:
    const AuthorizedMotorCommands& actuator_command() const noexcept { return actuator_command_; }
    AuthorizedMotorCommands take_actuator_command() noexcept {
        return std::move(actuator_command_);
    }
    FaultCode fault() const noexcept { return fault_; }
    bool inhibited() const noexcept { return inhibited_; }
    bool upper_limit_blocked() const noexcept { return upper_limit_blocked_; }
    bool lower_limit_blocked() const noexcept { return lower_limit_blocked_; }

private:
    SafetyResult(AuthorizedMotorCommands&& command, FaultCode fault,
                 bool inhibited, bool upper_blocked, bool lower_blocked) noexcept
        : actuator_command_(std::move(command)), fault_(fault), inhibited_(inhibited),
          upper_limit_blocked_(upper_blocked), lower_limit_blocked_(lower_blocked) {}

    AuthorizedMotorCommands actuator_command_;
    FaultCode fault_{FaultCode::NONE};
    bool inhibited_{true};
    bool upper_limit_blocked_{false};
    bool lower_limit_blocked_{false};

    friend class SafetyManager;
};

class SafetyManager {
public:
    std::uint32_t authorization_generation() const noexcept { return authorization_generation_; }

    SafetyResult enforce(const StateMachine& state_machine, const SystemState& state,
                         const SafetyRequirements& requirements,
                         const OperatorCommand& command,
                         const MotorMixResult& mix) noexcept;

private:
    std::uint32_t authorization_generation_{0};
};

}  // namespace raildrone
