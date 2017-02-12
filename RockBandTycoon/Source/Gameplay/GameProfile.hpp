#pragma once

#include "BandMember.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/Property.hpp"
#include <array>

class GameProfile
{
    VH_PROPERTY_RW(std::string, BandName);

public:
    GameProfile(int slot, const char* name = "");

    /* Save profile to disc */
    bool Save() const;

    /* Load profile from disc */
    bool Load();

    static bool GetProfilePath(std::string& outPath, int slot);

    BandMember* GetBandMember(eBandSlot::Type slot) const;

    int32_t GetMoney() const { return mMoney; }
    float GetSkill() const { return mSkill; }
    float GetPopularity() const { return mPopularity; }
    float GetReputation() const { return mReputation; }
    float GetQuality() const { return mQuality; }

    // Stat change delegates
    vh::MultiDelegate<> Money_OnChange;

private:
    static const uint32_t VERSION_TAG = 0x31b8a093;

    int mSlot;

    std::array<BandMember*, eBandSlot::MAX> mBandMembers;

    // Resources
    int32_t mMoney;

    // Stats
    float mSkill;
    float mPopularity;
    float mReputation;
    float mQuality;
};
