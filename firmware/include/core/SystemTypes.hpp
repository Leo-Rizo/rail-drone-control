#pragma once

#include <cstdint>

namespace raildrone {

enum class SystemMode : std::uint8_t {
    BOOT,
    SELF_TEST,
    DISARMED,
    ARMED,
    RUNNING,
    FAULT,
    EMERGENCY_STOP,
};

enum class ControlMode : std::uint8_t {
    NONE,
    MANUAL,
    ASSISTED,
    AUTOMATIC,
};

enum class FaultCode : std::uint16_t {
    NONE,
    SELF_TEST_FAILED,
    SAFETY_POLICY_UNCONFIGURED,
    INVALID_CONTROL_COMMAND,
    IMU_UNAVAILABLE,
    TOF_UNAVAILABLE,
    LIMIT_SWITCHES_UNAVAILABLE,
    GAMEPAD_LOST,
    GAMEPAD_STALE,
    UPPER_LIMIT,
    LOWER_LIMIT,
    INVALID_STATE,
    EMERGENCY_STOP,
};

// SI units are used throughout. Numeric values are meaningful only when the
// matching validity flag is true; default construction represents unavailable data.
struct SystemState {
    std::uint32_t timestamp_ms{0};

    float altitude_m{0.0F};
    bool altitude_valid{false};
    float angle_rad{0.0F};
    bool angle_valid{false};
    float angular_velocity_rad_s{0.0F};
    bool angular_velocity_valid{false};

    float battery_voltage{0.0F};
    bool battery_voltage_valid{false};

    bool imu_ok{false};
    bool tof_ok{false};
    bool upper_limit{false};
    bool upper_limit_valid{false};
    bool lower_limit{false};
    bool lower_limit_valid{false};
    bool gamepad_connected{false};
    bool gamepad_fresh{false};
};

// Manual axes are normalized to [-1, 1]. Positive vertical_command requests
// upward motion; the sign convention is provisional until the plant is characterized.
struct OperatorCommand {
    bool arm_requested{false};
    bool disarm_requested{false};
    bool run_requested{false};
    bool emergency_stop_requested{false};
    float altitude_reference_m{0.0F};
    bool altitude_reference_valid{false};
    float angle_reference_rad{0.0F};
    bool angle_reference_valid{false};
    float vertical_command{0.0F};
    float angular_command{0.0F};
    ControlMode requested_control_mode{ControlMode::NONE};
};

// Controller intent before motor allocation. Positive altitude_effort is an
// upward request; its physical mapping remains provisional.
struct ControlEffort {
    float altitude_effort{0.0F};
    float angle_effort{0.0F};

    ControlEffort(float altitude = 0.0F, float angle = 0.0F) noexcept
        : altitude_effort(altitude), angle_effort(angle) {}
};

// Protocol-independent normalized actuator requests: 0 = minimum/stopped,
// 1 = maximum. This is not a guarantee of ESC behavior until the protocol is selected.
struct MotorCommands {
    float motor_1{0.0F};
    float motor_2{0.0F};

    MotorCommands(float first = 0.0F, float second = 0.0F) noexcept
        : motor_1(first), motor_2(second) {}
};

struct TelemetryRecord {
    std::uint32_t timestamp_ms{0};
    SystemMode system_state{SystemMode::BOOT};
    ControlMode control_mode{ControlMode::NONE};
    float altitude_m{0.0F};
    bool altitude_valid{false};
    float altitude_reference_m{0.0F};
    bool altitude_reference_valid{false};
    float angle_rad{0.0F};
    bool angle_valid{false};
    float angle_reference_rad{0.0F};
    bool angle_reference_valid{false};
    float angular_velocity_rad_s{0.0F};
    bool angular_velocity_valid{false};
    float motor_1_command{0.0F};
    float motor_2_command{0.0F};
    float battery_voltage{0.0F};
    bool battery_voltage_valid{false};
    bool imu_ok{false};
    bool tof_ok{false};
    bool upper_limit{false};
    bool upper_limit_valid{false};
    bool lower_limit{false};
    bool lower_limit_valid{false};
    bool gamepad_connected{false};
    bool gamepad_fresh{false};
    FaultCode fault_code{FaultCode::NONE};
};

}  // namespace raildrone
