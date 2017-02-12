#pragma once

#include "BandMember.hpp"
#include <array>

class GameProfile
{
public:
    GameProfile(int slot, const char* name = "");

    /* Save profile to disc */
    bool Save();

    /* Load profile from disc */
    bool Load();

    const char* GetBandName();

    static bool GetProfilePath(std::string& outPath, int slot);

    BandMember* GetBandMember(eBandSlot::Type slot);

    int32_t GetMoney() const { return mMoney; }
    float GetSkill() const { return mSkill; }
    float GetPopularity() const { return mPopularity; }
    float GetReputation() const { return mReputation; }
    float GetQuality() const { return mQuality; }

private:
    static const uint32_t VERSION_TAG = 0x31b8a093;

    int mSlot;

    std::string mBandName;

    std::array<BandMember*, eBandSlot::MAX> mBandMembers;

    // Resources
    int32_t mMoney;

    // Stats
    float mSkill;
    float mPopularity;
    float mReputation;
    float mQuality;
};
