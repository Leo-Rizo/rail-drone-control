#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

TelemetryRecord make_telemetry(const SystemState& state, SystemMode mode,
                               ControlMode control_mode, const OperatorCommand& reference,
                               const MotorCommands& motors, FaultCode fault) noexcept;

}  // namespace raildrone
