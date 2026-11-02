#pragma once

#include <cstdint>

namespace raildrone {
namespace config {

constexpr std::uint32_t kSerialBaudRate = 115200;
constexpr std::uint32_t kHeartbeatIntervalMs = 1000;
constexpr float kMinimumMotorCommand = 0.0F;
constexpr float kMaximumMotorCommand = 1.0F;

}  // namespace config
}  // namespace raildrone
