#include "telemetry/TelemetryBuilder.hpp"

namespace raildrone {

TelemetryRecord make_telemetry(const SystemState& state, SystemMode mode,
                               ControlMode control_mode, const OperatorCommand& reference,
                               const AuthorizedMotorCommands& motors, FaultCode fault) noexcept {
    TelemetryRecord record{};
    record.timestamp_ms = state.timestamp_ms;
    record.system_state = mode;
    record.control_mode = control_mode;
    record.altitude_m = state.altitude_m;
    record.altitude_valid = state.altitude_valid;
    record.altitude_reference_m = reference.altitude_reference_m;
    record.altitude_reference_valid = reference.altitude_reference_valid;
    record.angle_rad = state.angle_rad;
    record.angle_valid = state.angle_valid;
    record.angle_reference_rad = reference.angle_reference_rad;
    record.angle_reference_valid = reference.angle_reference_valid;
    record.angular_velocity_rad_s = state.angular_velocity_rad_s;
    record.angular_velocity_valid = state.angular_velocity_valid;
    record.motor_1_command = motors.motor_1();
    record.motor_2_command = motors.motor_2();
    record.battery_voltage = state.battery_voltage;
    record.battery_voltage_valid = state.battery_voltage_valid;
    record.imu_ok = state.imu_ok;
    record.tof_ok = state.tof_ok;
    record.upper_limit = state.upper_limit;
    record.upper_limit_valid = state.upper_limit_valid;
    record.lower_limit = state.lower_limit;
    record.lower_limit_valid = state.lower_limit_valid;
    record.gamepad_connected = state.gamepad_connected;
    record.gamepad_fresh = state.gamepad_fresh;
    record.fault_code = fault;
    return record;
}

}  // namespace raildrone
