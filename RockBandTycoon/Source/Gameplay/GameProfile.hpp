#pragma once

#include "BandMember.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/Property.hpp"
#include "Gameplay/Shop.hpp"
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

    VH_PROPERTY_RW(BandMember, Guitarist);
    VH_PROPERTY_RW(BandMember, Bassist);
    VH_PROPERTY_RW(BandMember, Drummer);

    VH_PROPERTY_RW(Shop, Shop);

public:
    GameProfile(int slot, const char* name = "");

    /* Save profile to disc */
    bool Save() const;

    /* Load profile from disc */
    bool Load();

    static bool GetProfilePath(std::string& outPath, int slot);

    void AddMember(BandMember& member);
    const BandMember& GetBandMember(eBandSlot::Type type) const;

    void SetItem(eBandSlot::Type type, const Item& item);

private:
    static const uint32_t VERSION_TAG = 0x31bca0b6;

    int mSlot;
};
