#pragma once

#include "Item.hpp"
#include "Looks.hpp"

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
    friend class GameProfile;

    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(eBandSlot::Type, Type);
    VH_PROPERTY_RO(Looks, Looks);
    VH_PROPERTY_RO(Item, Item);

public:
    BandMember() {}
    BandMember(eBandSlot::Type type, const char* name, Item item, Looks looks)
        : mType(type), mName(name), mItem(item), mLooks(looks)
    {
    }
    BandMember(BandMember&&) = default;
    BandMember(const BandMember&) = default;
    BandMember& operator=(const BandMember&) = default;

};
