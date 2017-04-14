#pragma once

#include "Looks.hpp"

class HireItem
{
    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(Looks, Looks);
    VH_PROPERTY_RO(int32_t, Cost);

public:
    HireItem() {}
    HireItem(const std::string& name, const Looks& looks, int32_t cost)
        : mName(name), mLooks(looks), mCost(cost)
    {
    }
};
