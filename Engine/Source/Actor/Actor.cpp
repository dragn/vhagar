#include "Actor.hpp"
#include "Common.hpp"
#include <glm/gtx/rotate_vector.hpp>

namespace vh {

void Actor::SetPos(const V3& pos) {
    mPos = pos;
    _RecalcModel();
}

void Actor::AddPos(const V3& pos) {
    mPos += pos;
    _RecalcModel();
}

const V3& Actor::GetPos() const
{
    return mPos;
}

void Actor::SetScale(const V3& scale) {
    mScale = scale;
    _RecalcModel();
}

void Actor::AddScale(const V3& scale) {
    mScale += scale;
    _RecalcModel();
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
    mPitch = Math::ClampAngle(pitch);
    _RecalcModel();
}

void Actor::AddPitch(float pitch) {
    mPitch += pitch;
    mPitch = Math::ClampAngle(mPitch);
    _RecalcModel();
}

float Actor::GetYaw() const
{
    return mYaw;
}

void Actor::SetYaw(float yaw) {
    mYaw = Math::ClampAngle(yaw);
    _RecalcModel();
}

void Actor::AddYaw(float yaw) {
    mYaw += yaw;
    mYaw = Math::ClampAngle(mYaw);
    _RecalcModel();
}

const M4& Actor::GetTransform() const
{
    return mTransform;
}

const vh::Renderable* Actor::GetRenderable() const
{
    return mRenderable;
}

V3 Actor::GetForward() const
{
    return V3(mRot * glm::vec4(0, 0, -1, 0));
}

V3 Actor::GetUp() const
{
    return V3(mRot * glm::vec4(0, 1, 0, 0));
}

void Actor::SetRot(Rot rot) {
    mYaw = rot.yaw;
    mPitch = rot.pitch;
    _RecalcModel();
}

Rot Actor::GetRot() const {
    return Rot(mYaw, mPitch, 0.0f);
}

void Actor::AddRot(Rot rot) {
    mYaw += rot.yaw;
    mPitch += rot.pitch;
    _RecalcModel();
}

void Actor::_RecalcModel() {
    mRot = glm::rotate(glm::rotate(M4(1.f), - mYaw, V3(0, 1, 0)), mPitch, V3(1, 0, 0));
    mTransform = glm::translate(M4(1.f), mPos) * mRot * glm::scale(M4(1.f), mScale);
    //  modelMatrix = glm::translate(glm::scale(mRot, mScale), mPos);
}

} // namespace vh
