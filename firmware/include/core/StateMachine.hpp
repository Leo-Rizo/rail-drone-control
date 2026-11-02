#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

class ApplicationCoordinator;

class StateMachine {
public:
    SystemMode mode() const noexcept { return mode_; }
    FaultCode fault() const noexcept { return fault_; }
    std::uint32_t transition_generation() const noexcept { return transition_generation_; }
    bool motor_authority_allowed() const noexcept { return mode_ == SystemMode::RUNNING; }

    bool begin_self_test() noexcept;
    bool complete_self_test(bool checks_passed) noexcept;
    bool request_arm(bool interlocks_healthy) noexcept;
    bool request_run(bool operating_conditions_healthy) noexcept;
    bool request_disarm() noexcept;
    bool report_fault(FaultCode fault) noexcept;
    bool request_emergency_stop() noexcept;
    bool reset_fault(bool cause_cleared, bool authorized) noexcept;
    bool reset_emergency_stop(bool cause_inspected, bool input_released,
                              bool authorized) noexcept;

private:
    StateMachine() noexcept = default;

    bool transition_to(SystemMode next) noexcept;

    SystemMode mode_{SystemMode::BOOT};
    FaultCode fault_{FaultCode::NONE};
    std::uint32_t transition_generation_{0};

    friend class ApplicationCoordinator;
};

}  // namespace raildrone
