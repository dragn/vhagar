#pragma once

#include "Utils/Property.hpp"
#include "BandMember.hpp"

class ShopItem
{
    VH_PROPERTY_RO(eBandSlot::Type, Type);
    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(std::string, Img);
    VH_PROPERTY_RO(int32_t, Cost);
    VH_PROPERTY_RO(float, Reputation);
    VH_PROPERTY_RO(float, Popularity);
    VH_PROPERTY_RO(float, Quality);

public:
    ShopItem() {};
    ShopItem(eBandSlot::Type type, const std::string& name,
        const std::string& img, int32_t cost, float rep, float pop, float qual)
        : mType(type), mName(name), mImg(img), mCost(cost)
        , mReputation(rep), mPopularity(pop), mQuality(qual)
    {
    }
};
