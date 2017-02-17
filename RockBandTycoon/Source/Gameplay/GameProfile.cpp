#include "PCH.hpp"

#include "GameProfile.hpp"
#include <fstream>

GameProfile::GameProfile(int slot, const char* name /* = "" */)
    : mSlot(slot)
    , mBandName(name)
    , mMoney(0)
    , mDay(0)
    , mSkill(0)
    , mPopularity(0)
    , mReputation(0)
    , mQuality(0)
{
    mBandMembers = { nullptr };
}

bool GameProfile::GetProfilePath(std::string& outPath, int slot)
{
    const char* path = SDL_GetPrefPath(ORG_NAME, APP_NAME);
    if (!path)
    {
        LOG(ERROR) << "Could not get preferences folder" << SDL_GetError();
        return false;
    }

    outPath = path;

    const char* filename;
    switch (slot)
    {
    case 1: filename = "profile1.dat"; break;
    case 2: filename = "profile2.dat"; break;
    case 3: filename = "profile3.dat"; break;
    default:
        LOG(ERROR) << "Invalid slot: " << slot;
        return false;
    }

    outPath.append(filename);

    return true;
}

BandMember* GameProfile::GetBandMember(eBandSlot::Type slot) const
{
    return mBandMembers[slot];
}

template<typename T>
void Write(std::ostream& s, const T& val)
{
    s.write((char*) &val, sizeof(T));
}

template<typename T>
void Read(std::istream& s, T& val)
{
    s.read((char*) &val, sizeof(T));
}

template<> void Write(std::ostream& file, const std::string& str)
{
    std::string::size_type sz = str.size();
    file.write((char*) &sz, sizeof(std::string::size_type));
    file.write(str.c_str(), str.size());
}

template<> void Read(std::istream& file, std::string& outStr)
{
    std::string::size_type sz;
    file.read((char*) &sz, sizeof(std::string::size_type));
    char* buf = new char[sz];
    file.read(buf, sz);
    outStr.assign(buf, sz);
}

bool GameProfile::Save() const
{
    std::string fullPath;
    if (!GetProfilePath(fullPath, mSlot)) return false;

    std::fstream file(fullPath.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!file.is_open())
    {
        LOG(ERROR) << "Could not open profile file '" << fullPath << "'";
        return false;
    }

    // right out all profile
    Write(file, VERSION_TAG);

    Write(file, mBandName);
    Write(file, mMoney);
    Write(file, mPopularity);
    Write(file, mQuality);
    Write(file, mReputation);
    Write(file, mSkill);

    file.close();

    return true;
}

bool GameProfile::Load()
{
    std::string fullPath;
    if (!GetProfilePath(fullPath, mSlot)) return false;

    std::fstream file(fullPath.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
    {
        LOG(ERROR) << "Could not open profile file '" << fullPath << "'";
        return false;
    }

    uint32_t tag;
    Read(file, tag);
    if (tag != VERSION_TAG)
    {
        LOG(ERROR) << "Version tag mismatch: " << tag;
        file.close();
        return false;
    }

    Read(file, mBandName);
    Read(file, mMoney);
    Read(file, mPopularity);
    Read(file, mQuality);
    Read(file, mReputation);
    Read(file, mSkill);

    file.close();

    return true;
}
