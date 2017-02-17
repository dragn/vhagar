#pragma once

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
public:
    BandMember() {}
    BandMember(const char* name, const char* hair, const char* face,
        const char* body, const char* hands, const char* legs)
        : mName(name)
        , mHair(hair)
        , mFace(face)
        , mBody(body)
        , mHands(hands)
        , mLegs(legs)
    {
    }

    const std::string& GetName() const { return mName; }

    const std::string& GetHair() const { return mHair; }
    const std::string& GetFace() const { return mFace; }
    const std::string& GetBody() const { return mBody; }
    const std::string& GetHands() const { return mHands; }
    const std::string& GetLegs() const { return mLegs; }

private:
    std::string mName;
    eBandSlot::Type mSlot;

    std::string mHair;
    std::string mFace;
    std::string mBody;
    std::string mHands;
    std::string mLegs;
};
