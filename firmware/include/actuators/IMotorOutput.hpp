#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

class IMotorOutput {
public:
    virtual ~IMotorOutput() = default;
    virtual bool write(const MotorCommands& commands) = 0;
    virtual void inhibit() = 0;
};

}  // namespace raildrone
