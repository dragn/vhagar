#pragma once

#include "Core/Actor/Actor.hpp"
#include "Core/Factory.hpp"
#include "Core/standard.hpp"

namespace vh
{

class ActorFactory : public Factory<std::string, Actor>
{
public:
    ActorFactory();
};

} // namespace vh
