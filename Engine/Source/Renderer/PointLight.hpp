#pragma once

namespace vh {

class PointLight
{
public:
    virtual V3 GetPos() const = 0;
    virtual float GetIntensity() const = 0;
};

} // namespace vh
