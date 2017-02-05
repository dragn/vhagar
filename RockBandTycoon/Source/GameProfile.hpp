#pragma once

class GameProfile
{
public:
    GameProfile(int slot);

    /* Save profile to disc */
    bool Save();

    /* Load profile from disc */
    bool Load();

private:
    int mSlot;

    std::string mBandName;
};
