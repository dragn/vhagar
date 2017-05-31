#pragma once

namespace vh
{

class Actor;

class ActorBehavior
{
public:
    ActorBehavior(Actor* owner) : mOwner(owner) {}
    virtual ~ActorBehavior() {}

    // Called when actor enters a World
    virtual void StartPlay() {}

    // Called when actor leaves a world
    virtual void EndPlay() {}

    virtual void Tick(uint32_t delta) {}

    const Actor* GetOwner() const { return mOwner; }

protected:
    Actor* mOwner;
};

} // namespace vh
