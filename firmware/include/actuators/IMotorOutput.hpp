#pragma once

#include "safety/SafetyManager.hpp"

namespace raildrone {

class IMotorOutput {
public:
    IMotorOutput(const StateMachine& state_machine, const SafetyManager& safety_manager) noexcept
        : state_machine_(state_machine), safety_manager_(safety_manager) {}

    virtual ~IMotorOutput() = default;

    bool write(AuthorizedMotorCommands&& commands) {
        const bool current_authority =
            commands.authorized_ && commands.state_machine_ == &state_machine_ &&
            commands.safety_manager_ == &safety_manager_ &&
            commands.state_generation_ == state_machine_.transition_generation() &&
            commands.safety_generation_ == safety_manager_.authorization_generation() &&
            state_machine_.motor_authority_allowed();
        commands.authorized_ = false;
        if (!current_authority) {
            inhibit_output();
            return false;
        }
        if (!write_normalized(commands.motor_1_, commands.motor_2_)) {
            inhibit_output();
            return false;
        }
        return true;
    }

    void inhibit() { inhibit_output(); }

protected:
    virtual bool write_normalized(float motor_1, float motor_2) = 0;
    virtual void inhibit_output() = 0;

private:
    const StateMachine& state_machine_;
    const SafetyManager& safety_manager_;
};

}  // namespace raildrone
