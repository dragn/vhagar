#include "PCH.hpp"

#include "GameProfile.hpp"
#include <fstream>

GameProfile::GameProfile(int slot, const char* name /* = "" */)
    : mSlot(slot)
    , mBandName(name)
{
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

void WriteString(std::fstream& file, const std::string& str)
{
    std::string::size_type sz = str.size();
    file.write((char*) &sz, sizeof(std::string::size_type));
    file.write(str.c_str(), str.size());
}

void ReadString(std::fstream& file, std::string& outStr)
{
    std::string::size_type sz;
    file.read((char*) &sz, sizeof(std::string::size_type));
    char* buf = new char[sz];
    file.read(buf, sz);
    outStr.assign(buf, sz);
}

bool GameProfile::Save()
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
    file << VERSION_TAG;
    WriteString(file, mBandName);
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
    file >> tag;

    if (tag != VERSION_TAG)
    {
        LOG(ERROR) << "Version tag mismatch: " << tag;
        file.close();
        return false;
    }

    ReadString(file, mBandName);
    file.close();

    return true;
}

const char* GameProfile::GetBandName()
{
    return mBandName.c_str();
}
