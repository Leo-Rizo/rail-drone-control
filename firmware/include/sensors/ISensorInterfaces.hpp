#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

struct ImuSample {
    float angle_rad{0.0F};
    float angular_velocity_rad_s{0.0F};
};

class IImu {
public:
    virtual ~IImu() = default;
    virtual bool read(ImuSample& sample) = 0;
};

class IHeightSensor {
public:
    virtual ~IHeightSensor() = default;
    virtual bool read(float& altitude_m) = 0;
};

class ILimitSwitches {
public:
    virtual ~ILimitSwitches() = default;
    virtual bool read(bool& upper_active, bool& lower_active) = 0;
};

}  // namespace raildrone
