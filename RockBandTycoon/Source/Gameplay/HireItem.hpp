#pragma once

#include "Looks.hpp"

class HireItem
{
    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(Looks, Looks);

public:
    HireItem() {}
    HireItem(const std::string& name, const Looks& looks)
        : mName(name), mLooks(looks)
    {
    }
};
