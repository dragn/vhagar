#pragma once

namespace vh
{

class Rot
{
public:
    Rot(float yaw, float pitch, float roll)
        : yaw(yaw), pitch(pitch), roll(roll)
    {}

    float yaw;
    float pitch;
    float roll;
};

} // namespace vh
