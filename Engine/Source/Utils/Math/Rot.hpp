#pragma once

namespace vh
{

class Rot
{
public:
    Rot(float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f)
        : yaw(yaw), pitch(pitch), roll(roll)
    {}

    float yaw;
    float pitch;
    float roll;
};

} // namespace vh
