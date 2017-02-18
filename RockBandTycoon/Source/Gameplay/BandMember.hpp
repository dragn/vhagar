#pragma once

#include "Item.hpp"

namespace eBandSlot
{
enum Type
{
    Drums = 0,
    Bass,
    Guitar,
    Vocal,
    Keyboard,
    MAX
};
}

class BandMember
{
    friend class GameProfile;

public:
    BandMember() {}
    BandMember(eBandSlot::Type type, const char* name, const char* hair, const char* face,
        const char* body, const char* hands, const char* legs)
        : mName(name)
        , mSlot(type)
        , mHair(hair)
        , mFace(face)
        , mBody(body)
        , mHands(hands)
        , mLegs(legs)
    {
    }

    const std::string& GetName() const { return mName; }
    const eBandSlot::Type GetType() const { return mSlot; }

    const std::string& GetHair() const { return mHair; }
    const std::string& GetFace() const { return mFace; }
    const std::string& GetBody() const { return mBody; }
    const std::string& GetHands() const { return mHands; }
    const std::string& GetLegs() const { return mLegs; }

private:
    std::string mName;
    eBandSlot::Type mSlot;

    Item mItem;

    std::string mHair;
    std::string mFace;
    std::string mBody;
    std::string mHands;
    std::string mLegs;
};
