#include "core/StateMachine.hpp"

namespace raildrone {

bool StateMachine::transition_to(SystemMode next) noexcept {
    bool allowed = false;
    switch (mode_) {
        case SystemMode::BOOT:
            allowed = next == SystemMode::SELF_TEST || next == SystemMode::FAULT ||
                      next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::SELF_TEST:
            allowed = next == SystemMode::DISARMED || next == SystemMode::FAULT ||
                      next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::DISARMED:
            allowed = next == SystemMode::ARMED || next == SystemMode::FAULT ||
                      next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::ARMED:
            allowed = next == SystemMode::DISARMED || next == SystemMode::RUNNING ||
                      next == SystemMode::FAULT || next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::RUNNING:
            allowed = next == SystemMode::DISARMED || next == SystemMode::FAULT ||
                      next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::FAULT:
            allowed = next == SystemMode::SELF_TEST || next == SystemMode::EMERGENCY_STOP;
            break;
        case SystemMode::EMERGENCY_STOP:
            allowed = next == SystemMode::SELF_TEST;
            break;
    }
    if (!allowed) {
        return false;
    }
    mode_ = next;
    return true;
}

bool StateMachine::begin_self_test() noexcept {
    return transition_to(SystemMode::SELF_TEST);
}

bool StateMachine::complete_self_test(bool checks_passed) noexcept {
    if (mode_ != SystemMode::SELF_TEST) {
        return false;
    }
    if (!checks_passed) {
        fault_ = FaultCode::SELF_TEST_FAILED;
        return transition_to(SystemMode::FAULT);
    }
    fault_ = FaultCode::NONE;
    return transition_to(SystemMode::DISARMED);
}

bool StateMachine::request_arm(bool interlocks_healthy) noexcept {
    return mode_ == SystemMode::DISARMED && interlocks_healthy &&
           transition_to(SystemMode::ARMED);
}

bool StateMachine::request_run(bool operating_conditions_healthy) noexcept {
    return mode_ == SystemMode::ARMED && operating_conditions_healthy &&
           transition_to(SystemMode::RUNNING);
}

bool StateMachine::request_disarm() noexcept {
    if (mode_ != SystemMode::ARMED && mode_ != SystemMode::RUNNING) {
        return false;
    }
    return transition_to(SystemMode::DISARMED);
}

bool StateMachine::report_fault(FaultCode fault) noexcept {
    if (mode_ == SystemMode::FAULT || mode_ == SystemMode::EMERGENCY_STOP ||
        fault == FaultCode::NONE) {
        return false;
    }
    fault_ = fault;
    return transition_to(SystemMode::FAULT);
}

bool StateMachine::request_emergency_stop() noexcept {
    if (mode_ == SystemMode::EMERGENCY_STOP) {
        return true;
    }
    fault_ = FaultCode::EMERGENCY_STOP;
    return transition_to(SystemMode::EMERGENCY_STOP);
}

bool StateMachine::reset_fault(bool cause_cleared, bool authorized) noexcept {
    if (mode_ != SystemMode::FAULT || !cause_cleared || !authorized) {
        return false;
    }
    fault_ = FaultCode::NONE;
    return transition_to(SystemMode::SELF_TEST);
}

bool StateMachine::reset_emergency_stop(bool cause_inspected, bool input_released,
                                        bool authorized) noexcept {
    if (mode_ != SystemMode::EMERGENCY_STOP || !cause_inspected || !input_released ||
        !authorized) {
        return false;
    }
    fault_ = FaultCode::NONE;
    return transition_to(SystemMode::SELF_TEST);
}

}  // namespace raildrone
