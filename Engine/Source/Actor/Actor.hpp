#pragma once

#include "Common.hpp"

namespace vh {

/**
 * Actor is something that could be placed in Scene,
 * something with position and rotation, but not necessarily drawable or movable.
 */
class Actor {

public:
    Actor() :
        mPos(V3(0, 0, 0)),
        mScale(V3(1, 1, 1)),
        mYaw(0.0f),
        mPitch(0.0f)
    {
        _UpdateTransform();
    }

    Actor(V3 pos) :
        mPos(pos),
        mScale(V3(1, 1, 1)),
        mYaw(0.0f),
        mPitch(0.0f)
    {
        _UpdateTransform();
    }

    Actor(V3 pos, Rot rot) :
        mPos(pos),
        mScale(V3(1, 1, 1)),
        mYaw(rot.yaw),
        mPitch(rot.pitch)
    {
        _UpdateTransform();
    }

    virtual ~Actor() {};

    // Called when actor is created
    virtual void OnCreate() {};

    // Called when actor is destroyed
    virtual void OnDestroy() {};

    // Actor ticks are called from World
    virtual void Tick(uint32_t delta) {}

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
    void AddRot(Rot rot);
    void SetRot(Rot rot);

    // -- get Actor's forward vector
    V3 GetForward() const;

    // -- get Actor's up vector
    V3 GetUp() const;

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

protected:
    virtual void _UpdateTransform();

    M4 mTransform;

private:
    V3 mPos;
    V3 mScale;

    float mYaw = 0;
    float mPitch = 0;

    std::string mName;
};

} // namespace vh
