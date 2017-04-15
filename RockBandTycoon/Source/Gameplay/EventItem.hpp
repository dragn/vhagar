#pragma once

#include "utils/Property.hpp"

class EventItem
{
    VH_PROPERTY_RO(std::string, Name);
    VH_PROPERTY_RO(std::string, Description);
    VH_PROPERTY_RO(int32_t, Money);
    VH_PROPERTY_RO(float, Reputation);
    VH_PROPERTY_RO(float, Popularity);
    VH_PROPERTY_RO(float, Skill);

public:
    EventItem() {}
    EventItem(const std::string& name, const std::string& description, int32_t money,
        float rep, float pop, float skill)
        : mName(name), mDescription(description), mMoney(money)
        , mReputation(rep), mPopularity(pop), mSkill(skill)
    {}
};
