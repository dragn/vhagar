#pragma once

#include "BandMember.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/Property.hpp"
#include <array>

class GameProfile
{
    VH_PROPERTY_RW(std::string, BandName);
    VH_PROPERTY_RW(int32_t, Money);
    VH_PROPERTY_RW(uint32_t, Day);
    VH_PROPERTY_RW(float, Skill);
    VH_PROPERTY_RW(float, Reputation);
    VH_PROPERTY_RW(float, Popularity);
    VH_PROPERTY_RW(float, Quality);

public:
    GameProfile(int slot, const char* name = "");

    /* Save profile to disc */
    bool Save() const;

    /* Load profile from disc */
    bool Load();

    static bool GetProfilePath(std::string& outPath, int slot);

    BandMember* GetBandMember(eBandSlot::Type slot) const;

private:
    static const uint32_t VERSION_TAG = 0x31bca0b1;

    int mSlot;

    std::array<BandMember*, eBandSlot::MAX> mBandMembers;
};
