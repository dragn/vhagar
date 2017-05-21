#include "Common.hpp"

#include "Actor.hpp"
#include <glm/gtx/rotate_vector.hpp>

namespace vh {

void Actor::SetPos(const V3& pos) {
    mPos = pos;
    _UpdateTransform();
}

void Actor::AddPos(const V3& pos) {
    mPos += pos;
    _UpdateTransform();
}

Actor::~Actor()
{
    EndPlay();
}

const V3& Actor::GetPos() const
{
    return mPos;
}

void Actor::SetScale(const V3& scale) {
    mScale = scale;
    _UpdateTransform();
}

void Actor::AddScale(const V3& scale) {
    mScale += scale;
    _UpdateTransform();
}

const V3& Actor::GetScale() const
{
    return mScale;
}

float Actor::GetPitch() const
{
    return mPitch;
}

void Actor::SetPitch(float pitch) {
    mPitch = Math::WrapAngle(pitch);
    mQuat = Quat(V3(mPitch, mYaw, 0));
    _UpdateTransform();
}

void Actor::AddPitch(float pitch) {
    mPitch = Math::WrapAngle(mPitch + pitch);
    mQuat = Quat(V3(mPitch, mYaw, 0));
    _UpdateTransform();
}

vh::M4 Actor::GetView() const
{
    return glm::lookAt(GetPos(), GetPos() + GetForward(), GetUp());
}

void Actor::StartPlay()
{
    if (!mPlaying)
    {
        for (const std::unique_ptr<ActorBehavior>& behavior : mBehaviors)
        {
            behavior->StartPlay();
        }
        mPlaying = true;
    }
}

void Actor::EndPlay()
{
    if (mPlaying)
    {
        for (const std::unique_ptr<ActorBehavior>& behavior : mBehaviors)
        {
            behavior->EndPlay();
        }
        mPlaying = false;
    }
}

float Actor::GetYaw() const
{
    return mYaw;
}

void Actor::SetYaw(float yaw) {
    mYaw = Math::WrapAngle(yaw);
    mQuat = Quat(V3(mPitch, mYaw, 0));
    _UpdateTransform();
}

void Actor::AddYaw(float yaw) {
    mYaw = Math::WrapAngle(mYaw + yaw);
    mQuat = Quat(V3(mPitch, mYaw, 0));
    _UpdateTransform();
}

V3 Actor::GetForward() const
{
    M4 rot = glm::toMat4(mQuat);
    return V3(rot * glm::vec4(0, 0, -1, 0));
}

V3 Actor::GetUp() const
{
    return V3(glm::toMat4(mQuat) * glm::vec4(0, 1, 0, 0));
}


vh::V3 Actor::GetRight() const
{
    return V3(glm::toMat4(mQuat) * glm::vec4(1, 0, 0, 0));
}

void Actor::SetRot(Rot rot)
{
    mQuat = Quat(V3(rot.pitch, rot.yaw, rot.roll));
    _UpdateTransform();
}

void Actor::SetRot(V3 rot)
{
    mQuat = Quat(rot);
    _UpdateTransform();
}

void Actor::SetRot(Quat quat)
{
    mQuat = quat;
    _UpdateTransform();
}

Rot Actor::GetRot() const
{
    V3 euler = glm::eulerAngles(mQuat);
    return Rot(euler.y, euler.x, euler.z);
}

vh::Quat Actor::GetQuat() const
{
    return mQuat;
}

void Actor::AddRot(Rot rot)
{
    V3 euler = glm::eulerAngles(mQuat);
    euler.x = Math::WrapAngle(euler.x + rot.pitch);
    euler.y = Math::WrapAngle(euler.y + rot.yaw);
    euler.z = Math::WrapAngle(euler.z + rot.roll);
    mQuat = Quat(euler);
    _UpdateTransform();
}

void Actor::_UpdateTransform() {
    mTransform = glm::translate(M4(1.f), mPos) * glm::mat4_cast(mQuat) * glm::scale(M4(1.f), mScale);
    //  modelMatrix = glm::translate(glm::scale(mRot, mScale), mPos);
}

void Actor::Tick(uint32_t delta)
{
    if (mPlaying)
    {
        // tick all behaviors
        for (const std::unique_ptr<ActorBehavior>& behavior : mBehaviors)
        {
            behavior->Tick(delta);
        }
    }
}

Actor::Actor(World* world, V3 pos, Rot rot, V3 scale) :
    mOwner(world),
    mPlaying(false),
    mPos(pos),
    mScale(scale),
    mYaw(rot.yaw),
    mPitch(rot.pitch)
{
    mQuat = Quat(V3(mPitch, mYaw, 0));
    _UpdateTransform();
}

} // namespace vh
