#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

class MotorMixer {
public:
    // Provisional allocation only; base and efforts must already share a
    // normalized command scale. Physical thrust mapping and gains are TBD.
    MotorCommands mix(float base_command, const ControlEffort& effort) const noexcept;
};

}  // namespace raildrone
