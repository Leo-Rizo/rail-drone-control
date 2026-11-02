#pragma once

#include "core/StateMachine.hpp"
#include "control/MotorMixer.hpp"
#include "safety/SafetyManager.hpp"
#include "telemetry/TelemetryBuilder.hpp"

namespace raildrone {

struct ApplicationCycleResult {
    SafetyResult safety;
    TelemetryRecord telemetry;
};

// Owns the authoritative state machine and propagates safety faults before
// publishing telemetry. It does not own or write a physical motor driver.
class ApplicationCoordinator {
public:
    ApplicationCoordinator() noexcept = default;
    ApplicationCoordinator(const ApplicationCoordinator&) = delete;
    ApplicationCoordinator& operator=(const ApplicationCoordinator&) = delete;

    StateMachine& state_machine() noexcept { return state_machine_; }
    const StateMachine& state_machine() const noexcept { return state_machine_; }
    const SafetyManager& safety_manager() const noexcept { return safety_manager_; }

    ApplicationCycleResult update(const SystemState& state,
                                  const SafetyRequirements& requirements,
                                  const OperatorCommand& command,
                                  const MotorMixResult& mix,
                                  ControlMode active_control_mode) noexcept;

private:
    StateMachine state_machine_{};
    SafetyManager safety_manager_{};
};

}  // namespace raildrone
