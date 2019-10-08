#pragma once

#include "Core/Actor/ActorBehavior.hpp"
#include "Utils/Math.hpp"

namespace vh
{
    typedef size_t ActorID;
    static ActorID ActorID_Invalid = ActorID(-1);

    /**
     * Actor is something that could be placed in Scene,
     * something with position and rotation, but not necessarily drawable or movable.
     */
    class Actor : public std::enable_shared_from_this<Actor>
    {
        friend class MWorld;

    public:
        Actor(ActorID id, V3 pos = V3(), Rot rot = Rot(), V3 scale = V3(1.0f, 1.0f, 1.0f));
        virtual ~Actor();

        // -- id
        ActorID GetID() const { return mId; }

        // -- transform
        const M4& GetTransform() const { return mTransform; }

        // -- pos
        const V3& GetPos() const;
        void SetPos(const V3& pos);
        void AddPos(const V3& pos);

        // -- scale
        const V3& GetScale() const;
        void SetScale(const V3& scale);
        void AddScale(const V3& scale);

        // -- rot
        Rot GetRot() const;
        Quat GetQuat() const;
        void AddRot(Rot rot);
        void SetRot(Rot rot);
        void SetRot(V3 rot); // vec3 of euler angles
        void SetRot(Quat quat);

        // -- get Actor's forward vector
        V3 GetForward() const;

        // -- get Actor's up vector
        V3 GetUp() const;

        // -- get Actor's right vector
        V3 GetRight() const;

        // -- yaw
        float GetYaw() const;
        void SetYaw(float yaw);
        void AddYaw(float deltaYaw);

        // -- pitch
        float GetPitch() const;
        void SetPitch(float pitch);
        void AddPitch(float deltaPitch);

        void SetName(const std::string& name)
        {
            mName = name;
        }

        void SetName(const char* name)
        {
            mName = name;
        }

        const std::string& GetName() const
        {
            return mName;
        }

        M4 GetView() const;

        template<typename BEHAVIOR_CLASS, typename... ARGS>
        BEHAVIOR_CLASS* AddBehavior(ARGS... args)
        {
            return mRootBehavior.AddChild<BEHAVIOR_CLASS>(args...);
        }

        template<typename BEHAVIOR_CLASS, typename FUNC>
        void ForEachBehaviorOfType(FUNC func)
        {
            mRootBehavior.ForEachChildOfType<BEHAVIOR_CLASS>(func);
        }

        template<typename BEHAVIOR_CLASS>
        BEHAVIOR_CLASS* GetBehaviorOfType()
        {
            for (auto beh = mRootBehavior.mChildren.begin(); beh != mRootBehavior.mChildren.end(); beh++)
            {
                BEHAVIOR_CLASS* base = dynamic_cast<BEHAVIOR_CLASS*>(beh->get());
                if (base) return base;
            }
            return nullptr;
        }

        // -- Enable this actor
        void StartPlay();

        // -- Disable this actor
        void EndPlay();

    protected:
        virtual void _UpdateTransform();

        M4 mTransform;

    private:
        ActorID mId = ActorID_Invalid;

        bool mPlaying;

        V3 mPos;
        V3 mScale;

        float mYaw;
        float mPitch;
        Quat mQuat;

        std::string mName;

        ActorBehavior mRootBehavior;

        //std::list<std::unique_ptr<ActorBehavior>> mBehaviors;

        // Actor ticks are called from World
        void Tick(uint32_t delta);
    };

} // namespace vh
