#pragma once

#include "core/SystemTypes.hpp"

namespace raildrone {

class IGamepad {
public:
    virtual ~IGamepad() = default;
    virtual bool read(OperatorCommand& command) = 0;
};

}  // namespace raildrone
