#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

class MotorMixer;

class MotorMixResult {
public:
    const MotorCommands& commands() const noexcept { return commands_; }
    const ControlEffort& effort() const noexcept { return effort_; }
    FaultCode fault() const noexcept { return fault_; }
    bool valid() const noexcept { return fault_ == FaultCode::NONE; }

private:
    MotorMixResult(const MotorCommands& commands, const ControlEffort& effort,
                   FaultCode fault) noexcept
        : commands_(commands), effort_(effort), fault_(fault) {}

    MotorCommands commands_{};
    ControlEffort effort_{};
    FaultCode fault_{FaultCode::INVALID_CONTROL_COMMAND};

    friend class MotorMixer;
};

class MotorMixer {
public:
    // Provisional allocation only; base and efforts must already share a
    // normalized command scale. Physical thrust mapping and gains are TBD.
    MotorMixResult mix(float base_command, const ControlEffort& effort) const noexcept;
};

}  // namespace raildrone
