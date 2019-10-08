#pragma once

#include "Actor.hpp"
#include "Core/Factory.hpp"

namespace vh
{

class ActorFactory : public Factory<std::string, Actor>
{
public:
    ActorFactory();
};

} // namespace vh
