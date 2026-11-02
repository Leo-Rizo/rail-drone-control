#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>

#include <unity.h>

#include "actuators/IMotorOutput.hpp"
#include "control/MotorMixer.hpp"
#include "core/ApplicationCoordinator.hpp"

using namespace raildrone;

void setUp(void) {}
void tearDown(void) {}

template <typename Output, typename Argument>
class CanWriteArgument {
private:
    template <typename Candidate>
    static auto test(int) -> decltype(
        std::declval<Candidate&>().write(std::declval<Argument>()), std::true_type{});

    template <typename>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<Output>(0))::value;
};

static_assert(!std::is_constructible<AuthorizedMotorCommands, MotorCommands>::value,
              "raw motor commands must not construct an authorized command");
static_assert(!CanWriteArgument<IMotorOutput, MotorCommands>::value,
              "raw motor commands must not satisfy the actuator write interface");
static_assert(CanWriteArgument<IMotorOutput, AuthorizedMotorCommands>::value,
              "the actuator interface must accept safety-authorized commands");

namespace {

SafetyRequirements healthy_policy() {
    return SafetyRequirements::configured(true, true, true, true);
}

SystemState healthy_state() {
    SystemState state{};
    state.imu_ok = true;
    state.tof_ok = true;
    state.upper_limit_valid = true;
    state.lower_limit_valid = true;
    state.gamepad_connected = true;
    state.gamepad_fresh = true;
    return state;
}

bool enter_running(StateMachine& machine) {
    return machine.begin_self_test() && machine.complete_self_test(true) &&
           machine.request_arm(true) && machine.request_run(true);
}

class RecordingMotorOutput final : public IMotorOutput {
public:
    RecordingMotorOutput(const StateMachine& machine, const SafetyManager& safety)
        : IMotorOutput(machine, safety) {}

    int write_count{0};
    int inhibit_count{0};
    float last_motor_1{0.0F};
    float last_motor_2{0.0F};

protected:
    bool write_normalized(float motor_1, float motor_2) override {
        ++write_count;
        last_motor_1 = motor_1;
        last_motor_2 = motor_2;
        return true;
    }

    void inhibit_output() override { ++inhibit_count; }
};

void test_initial_state_is_boot_and_has_no_motor_authority() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_EQUAL(SystemMode::BOOT, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
}

void test_motor_authority_is_denied_outside_running() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.begin_self_test());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.complete_self_test(true));
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.request_arm(true));
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.report_fault(FaultCode::INVALID_STATE));
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
}

void test_valid_transition_path_reaches_running() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    TEST_ASSERT_EQUAL(SystemMode::RUNNING, app.state_machine().mode());
    TEST_ASSERT_TRUE(app.state_machine().motor_authority_allowed());
}

void test_invalid_transitions_are_rejected_without_state_change() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_FALSE(machine.request_arm(true));
    TEST_ASSERT_FALSE(machine.request_run(true));
    TEST_ASSERT_FALSE(machine.request_disarm());
    TEST_ASSERT_EQUAL(SystemMode::BOOT, machine.mode());
    TEST_ASSERT_TRUE(machine.begin_self_test());
    TEST_ASSERT_FALSE(machine.request_run(true));
    TEST_ASSERT_EQUAL(SystemMode::SELF_TEST, machine.mode());
}

void test_fault_inhibits_and_latches_motor_authority() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_TRUE(enter_running(machine));
    TEST_ASSERT_TRUE(machine.report_fault(FaultCode::TOF_UNAVAILABLE));
    TEST_ASSERT_EQUAL(SystemMode::FAULT, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_FALSE(machine.request_run(true));
}

void test_fault_recovery_requires_clear_and_authorized_reset() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_TRUE(machine.report_fault(FaultCode::IMU_UNAVAILABLE));
    TEST_ASSERT_FALSE(machine.begin_self_test());
    TEST_ASSERT_FALSE(machine.reset_fault(false, true));
    TEST_ASSERT_FALSE(machine.reset_fault(true, false));
    TEST_ASSERT_EQUAL(SystemMode::FAULT, machine.mode());
    TEST_ASSERT_TRUE(machine.reset_fault(true, true));
    TEST_ASSERT_EQUAL(SystemMode::SELF_TEST, machine.mode());
}

void test_emergency_stop_inhibits_and_cannot_self_clear() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_FALSE(machine.begin_self_test());
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_EQUAL(SystemMode::EMERGENCY_STOP, machine.mode());
}

void test_emergency_reset_requires_inspection_release_and_authority() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_FALSE(machine.reset_emergency_stop(false, true, true));
    TEST_ASSERT_FALSE(machine.reset_emergency_stop(true, false, true));
    TEST_ASSERT_FALSE(machine.reset_emergency_stop(true, true, false));
    TEST_ASSERT_TRUE(machine.reset_emergency_stop(true, true, true));
    TEST_ASSERT_EQUAL(SystemMode::SELF_TEST, machine.mode());
}

void test_failed_self_test_enters_fault_and_stays_inhibited() {
    ApplicationCoordinator app;
    auto& machine = app.state_machine();
    TEST_ASSERT_TRUE(machine.begin_self_test());
    TEST_ASSERT_TRUE(machine.complete_self_test(false));
    TEST_ASSERT_EQUAL(SystemMode::FAULT, machine.mode());
    TEST_ASSERT_EQUAL(FaultCode::SELF_TEST_FAILED, machine.fault());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
}

void test_unconfigured_safety_policy_fails_safe_and_propagates_fault() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    MotorMixer mixer;
    const auto output = app.update(healthy_state(), SafetyRequirements{}, OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::SAFETY_POLICY_UNCONFIGURED, output.telemetry.fault_code);
    TEST_ASSERT_EQUAL(SystemMode::FAULT, output.telemetry.system_state);
}

void test_configured_healthy_dependencies_allow_motor_evaluation() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    MotorMixer mixer;
    const auto output = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::ASSISTED);
    TEST_ASSERT_FALSE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(SystemMode::RUNNING, output.telemetry.system_state);
    TEST_ASSERT_EQUAL(ControlMode::ASSISTED, output.telemetry.control_mode);
}

void test_required_imu_failure_inhibits_and_faults() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.imu_ok = false;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::IMU_UNAVAILABLE, output.telemetry.fault_code);
    TEST_ASSERT_EQUAL(SystemMode::FAULT, output.telemetry.system_state);
}

void test_required_tof_failure_inhibits_and_faults() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.tof_ok = false;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::TOF_UNAVAILABLE, output.telemetry.fault_code);
}

void test_required_disconnected_gamepad_inhibits() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.gamepad_connected = false;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::GAMEPAD_LOST, output.telemetry.fault_code);
}

void test_required_stale_gamepad_inhibits() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.gamepad_fresh = false;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::GAMEPAD_STALE, output.telemetry.fault_code);
}

void test_fault_state_inhibits_safety_output() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(app.state_machine().report_fault(FaultCode::INVALID_STATE));
    MotorMixer mixer;
    const auto output = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(SystemMode::FAULT, output.telemetry.system_state);
}

void test_emergency_state_inhibits_safety_output() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(app.state_machine().request_emergency_stop());
    MotorMixer mixer;
    const auto output = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(SystemMode::EMERGENCY_STOP, output.telemetry.system_state);
    TEST_ASSERT_EQUAL(FaultCode::EMERGENCY_STOP, output.telemetry.fault_code);
}

void test_upper_limit_outward_request_inhibits_both_motor_outputs() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.upper_limit = true;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort(0.2F, 0.0F)),
                                  ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_TRUE(output.safety.upper_limit_blocked());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, output.safety.actuator_command().motor_1());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, output.safety.actuator_command().motor_2());
    TEST_ASSERT_EQUAL(FaultCode::NONE, output.telemetry.fault_code);
}

void test_upper_limit_escape_direction_can_pass() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.upper_limit = true;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort(-0.2F, 0.0F)),
                                  ControlMode::MANUAL);
    TEST_ASSERT_FALSE(output.safety.inhibited());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.3F, output.safety.actuator_command().motor_1());
}

void test_lower_limit_outward_request_inhibits_both_motor_outputs() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.lower_limit = true;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort(-0.2F, 0.0F)),
                                  ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_TRUE(output.safety.lower_limit_blocked());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, output.safety.actuator_command().motor_1());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, output.safety.actuator_command().motor_2());
    TEST_ASSERT_EQUAL(SystemMode::RUNNING, output.telemetry.system_state);
    TEST_ASSERT_EQUAL(FaultCode::NONE, output.telemetry.fault_code);
}

void test_lower_limit_escape_direction_can_pass() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    auto state = healthy_state();
    state.lower_limit = true;
    MotorMixer mixer;
    const auto output = app.update(state, healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.5F, ControlEffort(0.2F, 0.0F)),
                                  ControlMode::MANUAL);
    TEST_ASSERT_FALSE(output.safety.inhibited());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.7F, output.safety.actuator_command().motor_1());
}

void test_pure_vertical_effort_changes_motor_commands_equally() {
    MotorMixer mixer;
    const auto mix = mixer.mix(0.5F, ControlEffort(0.1F, 0.0F));
    TEST_ASSERT_TRUE(mix.valid());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.6F, mix.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.6F, mix.commands().motor_2);
}

void test_angle_effort_creates_differential_motor_commands() {
    MotorMixer mixer;
    const auto mix = mixer.mix(0.5F, ControlEffort(0.0F, 0.1F));
    TEST_ASSERT_TRUE(mix.valid());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.6F, mix.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.4F, mix.commands().motor_2);
}

void test_mixer_clamps_upper_and_lower_values() {
    MotorMixer mixer;
    const auto upper = mixer.mix(0.9F, ControlEffort(0.3F, -0.3F));
    const auto lower = mixer.mix(0.1F, ControlEffort(-0.3F, 0.0F));
    TEST_ASSERT_TRUE(upper.valid());
    TEST_ASSERT_TRUE(lower.valid());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.9F, upper.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 1.0F, upper.commands().motor_2);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, lower.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, lower.commands().motor_2);
}

void test_mixer_rejects_nan_and_safety_inhibits_result() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    MotorMixer mixer;
    const auto mix = mixer.mix(NAN, ControlEffort{});
    TEST_ASSERT_FALSE(mix.valid());
    TEST_ASSERT_EQUAL(FaultCode::INVALID_CONTROL_COMMAND, mix.fault());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, mix.commands().motor_1);
    const auto output = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                  mix, ControlMode::MANUAL);
    TEST_ASSERT_TRUE(output.safety.inhibited());
    TEST_ASSERT_EQUAL(FaultCode::INVALID_CONTROL_COMMAND, output.telemetry.fault_code);
}

void test_mixer_rejects_positive_infinity() {
    MotorMixer mixer;
    const auto mix = mixer.mix(0.5F, ControlEffort(0.0F, INFINITY));
    TEST_ASSERT_FALSE(mix.valid());
    TEST_ASSERT_EQUAL(FaultCode::INVALID_CONTROL_COMMAND, mix.fault());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, mix.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, mix.commands().motor_2);
}

void test_mixer_rejects_negative_infinity() {
    MotorMixer mixer;
    const auto mix = mixer.mix(0.5F, ControlEffort(-INFINITY, 0.0F));
    TEST_ASSERT_FALSE(mix.valid());
    TEST_ASSERT_EQUAL(FaultCode::INVALID_CONTROL_COMMAND, mix.fault());
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, mix.commands().motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, mix.commands().motor_2);
}

void test_mixer_rejects_finite_input_overflow() {
    MotorMixer mixer;
    const float largest = std::numeric_limits<float>::max();
    const auto mix = mixer.mix(largest, ControlEffort(largest, 0.0F));
    TEST_ASSERT_FALSE(mix.valid());
    TEST_ASSERT_EQUAL(FaultCode::INVALID_CONTROL_COMMAND, mix.fault());
}

void test_inactive_system_states_report_no_active_control_mode() {
    ApplicationCoordinator app;
    MotorMixer mixer;
    const auto mix = mixer.mix(0.0F, ControlEffort{});
    const auto policy = SafetyRequirements::configured(false, false, false, false);
    const auto verify_none = [&](SystemMode expected) {
        const auto output = app.update(healthy_state(), policy, OperatorCommand{}, mix,
                                       ControlMode::AUTOMATIC);
        TEST_ASSERT_EQUAL(expected, output.telemetry.system_state);
        TEST_ASSERT_EQUAL(ControlMode::NONE, output.telemetry.control_mode);
    };

    verify_none(SystemMode::BOOT);
    TEST_ASSERT_TRUE(app.state_machine().begin_self_test());
    verify_none(SystemMode::SELF_TEST);
    TEST_ASSERT_TRUE(app.state_machine().complete_self_test(true));
    verify_none(SystemMode::DISARMED);
    TEST_ASSERT_TRUE(app.state_machine().report_fault(FaultCode::INVALID_STATE));
    verify_none(SystemMode::FAULT);
    TEST_ASSERT_TRUE(app.state_machine().request_emergency_stop());
    verify_none(SystemMode::EMERGENCY_STOP);
}

void test_safety_fault_updates_state_and_telemetry_but_limit_does_not() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    MotorMixer mixer;
    auto state = healthy_state();
    state.tof_ok = false;
    const auto sensor_fault = app.update(
        state, healthy_policy(), OperatorCommand{}, mixer.mix(0.5F, ControlEffort{}),
        ControlMode::MANUAL);
    TEST_ASSERT_EQUAL(SystemMode::FAULT, sensor_fault.telemetry.system_state);
    TEST_ASSERT_EQUAL(FaultCode::TOF_UNAVAILABLE, sensor_fault.telemetry.fault_code);
    TEST_ASSERT_TRUE(sensor_fault.safety.inhibited());

    ApplicationCoordinator limit_app;
    TEST_ASSERT_TRUE(enter_running(limit_app.state_machine()));
    state = healthy_state();
    state.upper_limit = true;
    const auto blocked = limit_app.update(
        state, healthy_policy(), OperatorCommand{},
        mixer.mix(0.5F, ControlEffort(0.1F, 0.0F)), ControlMode::MANUAL);
    TEST_ASSERT_TRUE(blocked.safety.inhibited());
    TEST_ASSERT_EQUAL(SystemMode::RUNNING, blocked.telemetry.system_state);
    TEST_ASSERT_EQUAL(FaultCode::NONE, blocked.telemetry.fault_code);
}

void test_authorized_actuator_type_cannot_be_created_from_raw_commands() {
    TEST_ASSERT_FALSE((std::is_constructible<AuthorizedMotorCommands, MotorCommands>::value));
    TEST_ASSERT_FALSE((CanWriteArgument<IMotorOutput, MotorCommands>::value));
    TEST_ASSERT_TRUE((CanWriteArgument<IMotorOutput, AuthorizedMotorCommands>::value));
}

void test_authorized_output_is_bound_to_current_safety_evaluation_and_state() {
    ApplicationCoordinator app;
    TEST_ASSERT_TRUE(enter_running(app.state_machine()));
    RecordingMotorOutput output(app.state_machine(), app.safety_manager());
    MotorMixer mixer;
    auto cycle = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                            mixer.mix(0.4F, ControlEffort{}), ControlMode::MANUAL);
    auto command = cycle.safety.take_actuator_command();
    TEST_ASSERT_TRUE(output.write(std::move(command)));
    TEST_ASSERT_EQUAL(1, output.write_count);
    TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.4F, output.last_motor_1);

    TEST_ASSERT_FALSE(output.write(std::move(command)));
    TEST_ASSERT_EQUAL(1, output.write_count);
    TEST_ASSERT_EQUAL(1, output.inhibit_count);

    auto stale_cycle = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                  mixer.mix(0.4F, ControlEffort{}), ControlMode::MANUAL);
    auto stale_command = stale_cycle.safety.take_actuator_command();
    app.update(healthy_state(), healthy_policy(), OperatorCommand{},
               mixer.mix(0.5F, ControlEffort{}), ControlMode::MANUAL);
    TEST_ASSERT_FALSE(output.write(std::move(stale_command)));
    TEST_ASSERT_EQUAL(2, output.inhibit_count);

    auto revoked_cycle = app.update(healthy_state(), healthy_policy(), OperatorCommand{},
                                    mixer.mix(0.4F, ControlEffort{}), ControlMode::MANUAL);
    auto revoked_command = revoked_cycle.safety.take_actuator_command();
    TEST_ASSERT_TRUE(app.state_machine().request_emergency_stop());
    TEST_ASSERT_FALSE(output.write(std::move(revoked_command)));
    TEST_ASSERT_EQUAL(3, output.inhibit_count);
}

}  // namespace

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_boot_and_has_no_motor_authority);
    RUN_TEST(test_motor_authority_is_denied_outside_running);
    RUN_TEST(test_valid_transition_path_reaches_running);
    RUN_TEST(test_invalid_transitions_are_rejected_without_state_change);
    RUN_TEST(test_fault_inhibits_and_latches_motor_authority);
    RUN_TEST(test_fault_recovery_requires_clear_and_authorized_reset);
    RUN_TEST(test_emergency_stop_inhibits_and_cannot_self_clear);
    RUN_TEST(test_emergency_reset_requires_inspection_release_and_authority);
    RUN_TEST(test_failed_self_test_enters_fault_and_stays_inhibited);
    RUN_TEST(test_unconfigured_safety_policy_fails_safe_and_propagates_fault);
    RUN_TEST(test_configured_healthy_dependencies_allow_motor_evaluation);
    RUN_TEST(test_required_imu_failure_inhibits_and_faults);
    RUN_TEST(test_required_tof_failure_inhibits_and_faults);
    RUN_TEST(test_required_disconnected_gamepad_inhibits);
    RUN_TEST(test_required_stale_gamepad_inhibits);
    RUN_TEST(test_fault_state_inhibits_safety_output);
    RUN_TEST(test_emergency_state_inhibits_safety_output);
    RUN_TEST(test_upper_limit_outward_request_inhibits_both_motor_outputs);
    RUN_TEST(test_upper_limit_escape_direction_can_pass);
    RUN_TEST(test_lower_limit_outward_request_inhibits_both_motor_outputs);
    RUN_TEST(test_lower_limit_escape_direction_can_pass);
    RUN_TEST(test_pure_vertical_effort_changes_motor_commands_equally);
    RUN_TEST(test_angle_effort_creates_differential_motor_commands);
    RUN_TEST(test_mixer_clamps_upper_and_lower_values);
    RUN_TEST(test_mixer_rejects_nan_and_safety_inhibits_result);
    RUN_TEST(test_mixer_rejects_positive_infinity);
    RUN_TEST(test_mixer_rejects_negative_infinity);
    RUN_TEST(test_mixer_rejects_finite_input_overflow);
    RUN_TEST(test_inactive_system_states_report_no_active_control_mode);
    RUN_TEST(test_safety_fault_updates_state_and_telemetry_but_limit_does_not);
    RUN_TEST(test_authorized_actuator_type_cannot_be_created_from_raw_commands);
    RUN_TEST(test_authorized_output_is_bound_to_current_safety_evaluation_and_state);
    return UNITY_END();
}
