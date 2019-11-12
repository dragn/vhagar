#pragma once

#include "Core/standard.hpp"

namespace vh
{

class Actor;

class ActorBehavior
{
    friend class Actor;
    friend class MWorld;

    VH_PROPERTY_RW(V3, RelPos);
    VH_PROPERTY_RW(Quat, RelRot);
    VH_PROPERTY_RW(V3, RelScale);

private:
    static std::unordered_map<std::type_index, std::function<std::unique_ptr<ActorBehavior>()>>& GetOverrides();

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
    Quat GetRot();
    V3 GetScale();

    template<typename TYPE_FROM, typename TYPE_TO>
    static void AddOverride()
    {
        CHECK(GetOverrides().count(typeid(TYPE_FROM)) == 0);

        GetOverrides()[typeid(TYPE_FROM)] = []() { return std::make_unique<TYPE_TO>(); };
    }

    template<typename TYPE_FROM>
    static void RemoveOverride()
    {
        GetOverrides().erase(typeid(TYPE_FROM));
    }

    template<typename BEHAVIOR_CLASS, typename... Args>
    BEHAVIOR_CLASS* AddChild(Args... args)
    {
        std::unique_ptr<ActorBehavior> behavior;

        // -- Check if any modules overrides this behavior
        if (GetOverrides().count(typeid(BEHAVIOR_CLASS)))
        {
            behavior = GetOverrides()[typeid(BEHAVIOR_CLASS)]();
        }

        // -- If not - construct the behavior using provided class
        if (!behavior)
        {
            behavior = std::make_unique<BEHAVIOR_CLASS>(args...);
        }
        
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
