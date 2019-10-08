#pragma once

#include "Actor.hpp"
#include "Factory.hpp"

namespace vh
{

class ActorFactory : public Factory<std::string, Actor>
{
public:
    ActorFactory();
};

} // namespace vh
