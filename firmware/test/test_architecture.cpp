#include <unity.h>

#include "control/MotorMixer.hpp"
#include "core/StateMachine.hpp"
#include "safety/SafetyManager.hpp"

using namespace raildrone;

void test_state_machine_starts_safe_and_rejects_invalid_transition() {
    StateMachine machine;
    TEST_ASSERT_EQUAL(SystemMode::BOOT, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_FALSE(machine.request_arm(true));
    TEST_ASSERT_EQUAL(SystemMode::BOOT, machine.mode());
}

void test_state_machine_fault_and_emergency_stop_inhibit_motors() {
    StateMachine machine;
    TEST_ASSERT_TRUE(machine.begin_self_test());
    TEST_ASSERT_TRUE(machine.complete_self_test(true));
    TEST_ASSERT_TRUE(machine.request_arm(true));
    TEST_ASSERT_TRUE(machine.request_run(true));
    TEST_ASSERT_TRUE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.report_fault(FaultCode::TOF_UNAVAILABLE));
    TEST_ASSERT_EQUAL(SystemMode::FAULT, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_EQUAL(SystemMode::EMERGENCY_STOP, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
}

void test_state_machine_emergency_reset_requires_inspection_and_release() {
    StateMachine machine;
    TEST_ASSERT_TRUE(machine.request_emergency_stop());
    TEST_ASSERT_FALSE(machine.reset_emergency_stop(true, false, true));
    TEST_ASSERT_TRUE(machine.reset_emergency_stop(true, true, true));
    TEST_ASSERT_EQUAL(SystemMode::SELF_TEST, machine.mode());
    TEST_ASSERT_FALSE(machine.motor_authority_allowed());
}

void test_safety_blocks_upward_motion_at_upper_limit_and_allows_escape() {
    SafetyManager safety;
    SystemState state{};
    state.imu_ok = true;
    state.tof_ok = true;
    state.upper_limit = true;
    OperatorCommand command{};
    ControlEffort effort{0.2F, 0.0F};
    MotorCommands requested{0.7F, 0.7F};

    auto result = safety.enforce(state, SystemMode::RUNNING, {}, command, effort, requested);
    TEST_ASSERT_TRUE(result.inhibited);
    TEST_ASSERT_TRUE(result.upper_limit_blocked);
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.0F, result.motor_commands.motor_1);

    effort.altitude_effort = -0.2F;
    result = safety.enforce(state, SystemMode::RUNNING, {}, command, effort, requested);
    TEST_ASSERT_FALSE(result.inhibited);
    TEST_ASSERT_FALSE(result.upper_limit_blocked);
}

void test_safety_blocks_downward_motion_at_lower_limit_and_allows_escape() {
    SafetyManager safety;
    SystemState state{};
    state.lower_limit = true;
    OperatorCommand command{};
    ControlEffort effort{-0.2F, 0.0F};
    MotorCommands requested{0.4F, 0.4F};

    auto result = safety.enforce(state, SystemMode::RUNNING, {}, command, effort, requested);
    TEST_ASSERT_TRUE(result.inhibited);
    TEST_ASSERT_TRUE(result.lower_limit_blocked);

    effort.altitude_effort = 0.2F;
    result = safety.enforce(state, SystemMode::RUNNING, {}, command, effort, requested);
    TEST_ASSERT_FALSE(result.inhibited);
}

void test_safety_inhibits_armed_and_emergency_stop_outputs() {
    SafetyManager safety;
    SystemState state{};
    OperatorCommand command{};
    ControlEffort effort{};
    MotorCommands requested{0.8F, 0.8F};

    auto result = safety.enforce(state, SystemMode::ARMED, {}, command, effort, requested);
    TEST_ASSERT_TRUE(result.inhibited);

    command.emergency_stop_requested = true;
    result = safety.enforce(state, SystemMode::RUNNING, {}, command, effort, requested);
    TEST_ASSERT_TRUE(result.inhibited);
    TEST_ASSERT_EQUAL(FaultCode::EMERGENCY_STOP, result.fault);
}

void test_safety_sensor_and_gamepad_health_are_injectable() {
    SafetyManager safety;
    SystemState state{};
    OperatorCommand command{};
    ControlEffort effort{};
    MotorCommands requested{0.5F, 0.5F};
    SafetyRequirements requirements{};
    requirements.imu_required = true;

    auto result = safety.enforce(state, SystemMode::RUNNING, requirements,
                                 command, effort, requested);
    TEST_ASSERT_EQUAL(FaultCode::IMU_UNAVAILABLE, result.fault);
    TEST_ASSERT_TRUE(result.inhibited);

    requirements.imu_required = false;
    requirements.operator_input_required = true;
    result = safety.enforce(state, SystemMode::RUNNING, requirements,
                            command, effort, requested);
    TEST_ASSERT_EQUAL(FaultCode::GAMEPAD_LOST, result.fault);

    requirements.operator_input_required = false;
    requirements.limit_switches_required = true;
    result = safety.enforce(state, SystemMode::RUNNING, requirements,
                            command, effort, requested);
    TEST_ASSERT_EQUAL(FaultCode::LIMIT_SWITCHES_UNAVAILABLE, result.fault);
}

void test_mixer_adds_vertical_equally_and_angle_differentially() {
    MotorMixer mixer;
    auto output = mixer.mix(0.5F, ControlEffort{0.1F, 0.0F});
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.6F, output.motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.6F, output.motor_2);

    output = mixer.mix(0.5F, ControlEffort{0.0F, 0.1F});
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.6F, output.motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.4F, output.motor_2);
}

void test_mixer_clamps_normalized_outputs() {
    MotorMixer mixer;
    const auto output = mixer.mix(0.9F, ControlEffort{0.3F, -0.3F});
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.9F, output.motor_1);
    TEST_ASSERT_FLOAT_WITHIN(0.001F, 1.0F, output.motor_2);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_state_machine_starts_safe_and_rejects_invalid_transition);
    RUN_TEST(test_state_machine_fault_and_emergency_stop_inhibit_motors);
    RUN_TEST(test_state_machine_emergency_reset_requires_inspection_and_release);
    RUN_TEST(test_safety_blocks_upward_motion_at_upper_limit_and_allows_escape);
    RUN_TEST(test_safety_blocks_downward_motion_at_lower_limit_and_allows_escape);
    RUN_TEST(test_safety_inhibits_armed_and_emergency_stop_outputs);
    RUN_TEST(test_safety_sensor_and_gamepad_health_are_injectable);
    RUN_TEST(test_mixer_adds_vertical_equally_and_angle_differentially);
    RUN_TEST(test_mixer_clamps_normalized_outputs);
    UNITY_END();
}

void loop() {}
