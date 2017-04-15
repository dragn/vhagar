#pragma once

#include "Looks.hpp"

class HireItem
{
    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(Looks, Looks);
    VH_PROPERTY_RO(int32_t, Cost);
    VH_PROPERTY_RO(float, Reputation);
    VH_PROPERTY_RO(float, Popularity);

public:
    HireItem() {}
    HireItem(const std::string& name, const Looks& looks, int32_t cost, float rep, float pop)
        : mName(name), mLooks(looks), mCost(cost), mReputation(rep), mPopularity(pop)
    {
    }
};
