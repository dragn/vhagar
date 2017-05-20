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
    _UpdateTransform();
}

void Actor::AddPitch(float pitch) {
    mPitch += pitch;
    mPitch = Math::WrapAngle(mPitch);
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
    _UpdateTransform();
}

void Actor::AddYaw(float yaw) {
    mYaw += yaw;
    mYaw = Math::WrapAngle(mYaw);
    _UpdateTransform();
}

V3 Actor::GetForward() const
{
    M4 rot = glm::rotate(glm::rotate(M4(1.f), - mYaw, V3(0, 1, 0)), mPitch, V3(1, 0, 0));
    return V3(rot * glm::vec4(0, 0, -1, 0));
}

V3 Actor::GetUp() const
{
    M4 rot = glm::rotate(glm::rotate(M4(1.f), - mYaw, V3(0, 1, 0)), mPitch, V3(1, 0, 0));
    return V3(rot * glm::vec4(0, 1, 0, 0));
}

void Actor::SetRot(Rot rot) {
    mYaw = rot.yaw;
    mPitch = rot.pitch;
    _UpdateTransform();
}

Rot Actor::GetRot() const {
    return Rot(mYaw, mPitch, 0.0f);
}

void Actor::AddRot(Rot rot) {
    mYaw += rot.yaw;
    mPitch += rot.pitch;
    _UpdateTransform();
}

void Actor::_UpdateTransform() {
    M4 rot = glm::rotate(glm::rotate(M4(1.f), - mYaw, V3(0, 1, 0)), mPitch, V3(1, 0, 0));
    mTransform = glm::translate(M4(1.f), mPos) * rot * glm::scale(M4(1.f), mScale);
    //  modelMatrix = glm::translate(glm::scale(mRot, mScale), mPos);
}

void Actor::Tick(uint32_t delta)
{
    // tick all behaviors
    for (const std::unique_ptr<ActorBehavior>& behavior : mBehaviors)
    {
        behavior->Tick(delta);
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
    _UpdateTransform();
}

} // namespace vh
