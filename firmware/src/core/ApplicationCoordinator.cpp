#include "core/ApplicationCoordinator.hpp"

#include <utility>

namespace raildrone {

ApplicationCycleResult ApplicationCoordinator::update(
    const SystemState& state, const SafetyRequirements& requirements,
    const OperatorCommand& command, const MotorMixResult& mix,
    ControlMode active_control_mode) noexcept {
    SafetyResult safety = safety_manager_.enforce(
        state_machine_, state, requirements, command, mix);

    if (safety.fault() == FaultCode::EMERGENCY_STOP) {
        state_machine_.request_emergency_stop();
    } else if (safety.fault() != FaultCode::NONE) {
        state_machine_.report_fault(safety.fault());
    }

    const ControlMode reported_mode =
        state_machine_.mode() == SystemMode::RUNNING ? active_control_mode : ControlMode::NONE;
    const auto telemetry = make_telemetry(
        state, state_machine_.mode(), reported_mode, command,
        safety.actuator_command(), state_machine_.fault());
    return {std::move(safety), telemetry};
}

}  // namespace raildrone
