#pragma once

#include "Common.hpp"
#include "../Renderable/Mesh.hpp"

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
        mRot(M4(1)),
        mYaw(0.0f),
        mPitch(0.0f)
    {
        _RecalcModel();
    };

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

    // -- get Actor's transform as a matric
    const M4& GetTransform() const;

    // -- check if Actor has a mesh
    const vh::Renderable* GetRenderable() const;

    // -- yaw
    float GetYaw() const;
    void SetYaw(float yaw);
    void AddYaw(float deltaYaw);

    // -- pitch
    float GetPitch() const;
    void SetPitch(float pitch);
    void AddPitch(float deltaPitch);

private:
    V3 mPos;
    V3 mScale;

    M4 mRot;

    float mYaw = 0;
    float mPitch = 0;

    M4 mTransform;

    void _RecalcModel();

protected:
    vh::Renderable *mRenderable;
};

} // namespace vh
