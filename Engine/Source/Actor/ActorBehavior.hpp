#pragma once

#include "Utils/Property.hpp"

namespace vh
{

class Actor;

class ActorBehavior
{
    friend class Actor;
    friend class World;

    VH_PROPERTY_RW(glm::vec3, RelPos);
    VH_PROPERTY_RW(glm::quat, RelRot);
    VH_PROPERTY_RW(glm::vec3, RelScale);

public:
    ActorBehavior()
        : mParent(nullptr)
        , mRelPos(0.0f)
        , mRelRot()
        , mRelScale(1.0f)
    {}
    virtual ~ActorBehavior() {}

    // Called when actor enters a World
    virtual void StartPlay() {}

    // Called when actor leaves a world
    virtual void EndPlay() {}

    virtual void Tick(uint32_t delta) {}

    std::weak_ptr<Actor> GetOwner() const { return mOwner; }

    V3 GetPos();
    glm::quat GetRot();
    V3 GetScale();

    template<typename BEHAVIOR_CLASS, typename... Args>
    BEHAVIOR_CLASS* AddChild(Args... args)
    {
        std::unique_ptr<ActorBehavior> behavior = std::make_unique<BEHAVIOR_CLASS>(args...);
        BEHAVIOR_CLASS* ptr = reinterpret_cast<BEHAVIOR_CLASS*>(behavior.get());
        behavior->Attach(mOwner, this);
        mChildren.push_back(std::forward<std::unique_ptr<ActorBehavior>>(behavior));
        return ptr;
    }

    template<typename BEHAVIOR_CLASS, typename FUNC>
    void ForEachChildOfType(FUNC func)
    {
        std::for_each(mChildren.begin(), mChildren.end(), [&func] (const std::unique_ptr<ActorBehavior>& behavior)
        {
            BEHAVIOR_CLASS* base = dynamic_cast<BEHAVIOR_CLASS*>(behavior.get());
            if (base)
            {
                func(base);
                base->ForEachChildOfType<BEHAVIOR_CLASS>(func);
            }
        });
    }

protected:
    std::shared_ptr<Actor> LockOwner() const
    {
        CHECK(!mOwner.expired());
        return mOwner.lock();
    }

private:
    ActorBehavior* mParent;
    std::list<std::unique_ptr<ActorBehavior>> mChildren;
    std::weak_ptr<Actor> mOwner;

    void Attach(std::weak_ptr<Actor> owner, ActorBehavior* parent)
    {
        mOwner = owner;
        mParent = parent;
    }
};

} // namespace vh
