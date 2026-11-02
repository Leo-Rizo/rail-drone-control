#include "control/MotorMixer.hpp"

#include <cmath>

#include "config/FirmwareConfig.hpp"

namespace raildrone {
namespace {

float clamp_finite(float value) noexcept {
    if (value < config::kMinimumMotorCommand) {
        return config::kMinimumMotorCommand;
    }
    if (value > config::kMaximumMotorCommand) {
        return config::kMaximumMotorCommand;
    }
    return value;
}

}  // namespace

MotorMixResult MotorMixer::mix(float base_command, const ControlEffort& effort) const noexcept {
    if (!std::isfinite(base_command) || !std::isfinite(effort.altitude_effort) ||
        !std::isfinite(effort.angle_effort)) {
        return MotorMixResult(MotorCommands{}, effort, FaultCode::INVALID_CONTROL_COMMAND);
    }

    const float first = base_command + effort.altitude_effort + effort.angle_effort;
    const float second = base_command + effort.altitude_effort - effort.angle_effort;
    if (!std::isfinite(first) || !std::isfinite(second)) {
        return MotorMixResult(MotorCommands{}, effort, FaultCode::INVALID_CONTROL_COMMAND);
    }

    return MotorMixResult(
        MotorCommands(clamp_finite(first), clamp_finite(second)), effort, FaultCode::NONE);
}

}  // namespace raildrone
