#pragma once

#include "../Actor/Actor.hpp"

namespace vh {

class Arm : public Actor {

public:
    Arm(float length) :
        mLength(length)
    {}

    Arm() :
        mLength(10.0f)
    {}

    float GetLength() const
    {
        return mLength;
    }

    void SetLength(float length)
    {
        mLength = length;
    }

    void AddLength(float delta)
    {
        mLength += delta;
    }

private:
    float mLength;

};

} // namespace vh
