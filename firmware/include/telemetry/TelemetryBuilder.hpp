#pragma once

#include "core/SystemTypes.hpp"
#include "safety/SafetyManager.hpp"

namespace raildrone {

TelemetryRecord make_telemetry(const SystemState& state, SystemMode mode,
                               ControlMode control_mode, const OperatorCommand& reference,
                               const AuthorizedMotorCommands& motors, FaultCode fault) noexcept;

}  // namespace raildrone
