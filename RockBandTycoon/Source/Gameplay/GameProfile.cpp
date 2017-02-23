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
    mGuitarist = BandMember(eBandSlot::Guitar, "", Item(), Looks());
    mBassist = BandMember(eBandSlot::Bass, "", Item(), Looks());
    mDrummer  = BandMember(eBandSlot::Drums, "", Item(), Looks());
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

void GameProfile::AddMember(BandMember& member)
{
    switch (member.GetType())
    {
    case eBandSlot::Bass:
        SetBassist(member);
        break;
    case eBandSlot::Guitar:
        SetGuitarist(member);
        break;
    case eBandSlot::Drums:
        SetDrummer(member);
        break;
    default:
        LOG(ERROR) << "Unsupported member type: " << member.GetType();
    }
}

const BandMember& GameProfile::GetBandMember(eBandSlot::Type type) const
{
    switch (type)
    {
    case eBandSlot::Bass:
        return mBassist;
    case eBandSlot::Guitar:
        return mGuitarist;
    case eBandSlot::Drums:
        return mDrummer;
    default:
        LOG(ERROR) << "Unsupported member type: " << type;
        return mGuitarist;
    }
}

/*
    Write to binary stream. Default implementation.
*/
template<typename T>
void Write(std::ostream& s, const T& val)
{
    s.write((char*) &val, sizeof(T));
}

/*
    Read from binary stream. Default implementation.
*/
template<typename T>
void Read(std::istream& s, T& val)
{
    s.read((char*) &val, sizeof(T));
}

/*
    Write: std::string
*/
template<> void Write(std::ostream& file, const std::string& str)
{
    std::string::size_type sz = str.size();
    file.write((char*) &sz, sizeof(std::string::size_type));
    file.write(str.c_str(), str.size());
}

/*
    Read: std::string
*/
template<> void Read(std::istream& file, std::string& outStr)
{
    std::string::size_type sz;
    file.read((char*) &sz, sizeof(std::string::size_type));
    char* buf = new char[sz];
    file.read(buf, sz);
    outStr.assign(buf, sz);
}

/*
    Write: Looks
*/
template<> void Write(std::ostream& file, const Looks& looks)
{
    Write(file, looks.GetHair());
    Write(file, looks.GetFace());
    Write(file, looks.GetBody());
    Write(file, looks.GetHands());
    Write(file, looks.GetLegs());
}

/*
    Read: Looks
*/
template<> void Read(std::istream& file, Looks& looks)
{
    std::string hair, face, body, hands, legs;
    Read(file, hair);
    Read(file, face);
    Read(file, body);
    Read(file, hands);
    Read(file, legs);
    looks = Looks(hair, face, body, hands, legs);
}

/*
    Write: Item
*/
template<> void Write(std::ostream& file, const Item& item)
{
    Write(file, item.GetName());
    Write(file, item.GetImg());
}

/*
    Read: Item
*/
template<> void Read(std::istream& file, Item& item)
{
    std::string name, img;
    Read(file, name);
    Read(file, img);
    item = Item(name, img);
}

/*
    Write: BandMember
*/
template<> void Write(std::ostream& file, const BandMember& bm)
{
    Write(file, bm.GetName());
    Write(file, bm.GetType());
    Write(file, bm.GetItem());
    Write(file, bm.GetLooks());
}

/*
    Read: BandMember
*/
template<> void Read(std::istream& file, BandMember& bm)
{
    std::string name;
    eBandSlot::Type type;
    Read(file, name);
    Read(file, type);
    Item item; Read(file, item);
    Looks looks; Read(file, looks);
    bm = BandMember(type, name.c_str(), item, looks);
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

    Write(file, mGuitarist);
    Write(file, mBassist);
    Write(file, mDrummer);

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

    Read(file, mGuitarist); CHECK(mGuitarist.GetType() == eBandSlot::Guitar);
    Read(file, mBassist); CHECK(mBassist.GetType() == eBandSlot::Bass);
    Read(file, mDrummer); CHECK(mDrummer.GetType() == eBandSlot::Drums);

    file.close();

    return true;
}
