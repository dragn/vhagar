#pragma once

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

private:
    static const uint32_t VERSION_TAG = 0x31b8a093;

    int mSlot;

    std::string mBandName;
};
