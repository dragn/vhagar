#pragma once

#include "Actor.hpp"
#include "Common.hpp"

namespace vh
{

class CameraActor : public Actor
{
public:
    M4 GetView() const;
};

} // namespace vh
