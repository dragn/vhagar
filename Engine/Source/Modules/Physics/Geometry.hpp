#pragma once

#include "Utils/Math.hpp"

namespace vh
{

enum class GeometryType
{
    Sphere,
    Capsule,
    Box
};

class Geometry
{
public:
    virtual GeometryType GetType() const = 0;
};

class SphereGeometry : public Geometry
{
public:
    SphereGeometry(float radius) : mRadius(radius) {};

    virtual GeometryType GetType() const override
    {
        return GeometryType::Sphere;
    }

    float GetRadius() const { return mRadius; }

private:
    float mRadius;
};

class BoxGeometry : public Geometry
{
public:
    BoxGeometry(V3 extents) : mExtents(extents) {};

    virtual GeometryType GetType() const override
    {
        return GeometryType::Box;
    }

    const V3& GetExtents() const { return mExtents; }

private:
    V3 mExtents;
};

class CapsuleGeometry : public Geometry
{
public:
    CapsuleGeometry(float radius, float halfHeight)
        : mRadius(radius)
        , mHalfHeight(halfHeight)
    {}

    virtual GeometryType GetType() const override
    {
        return GeometryType::Capsule;
    }

    float GetRadius() const { return mRadius; }
    float GetHalfHeight() const { return mHalfHeight; }

private:
    float mRadius;
    float mHalfHeight;
};

} // namespace vh
