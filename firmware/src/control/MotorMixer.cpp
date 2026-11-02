#include "control/MotorMixer.hpp"

#include "config/FirmwareConfig.hpp"

namespace raildrone {
namespace {

float clamp(float value) noexcept {
    if (value < config::kMinimumMotorCommand) {
        return config::kMinimumMotorCommand;
    }
    if (value > config::kMaximumMotorCommand) {
        return config::kMaximumMotorCommand;
    }
    return value;
}

}  // namespace

MotorCommands MotorMixer::mix(float base_command, const ControlEffort& effort) const noexcept {
    return {
        clamp(base_command + effort.altitude_effort + effort.angle_effort),
        clamp(base_command + effort.altitude_effort - effort.angle_effort),
    };
}

}  // namespace raildrone
