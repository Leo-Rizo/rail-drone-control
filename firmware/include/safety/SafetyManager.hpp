#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

struct SafetyRequirements {
    bool imu_required{false};
    bool tof_required{false};
    bool limit_switches_required{false};
    bool operator_input_required{false};
};

struct SafetyResult {
    MotorCommands motor_commands{};
    FaultCode fault{FaultCode::NONE};
    bool inhibited{true};
    bool upper_limit_blocked{false};
    bool lower_limit_blocked{false};
};

class SafetyManager {
public:
    SafetyResult enforce(const SystemState& state, SystemMode mode,
                         const SafetyRequirements& requirements,
                         const OperatorCommand& command,
                         const ControlEffort& effort,
                         const MotorCommands& requested) const noexcept;
};

}  // namespace raildrone
