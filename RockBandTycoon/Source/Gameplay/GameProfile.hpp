#pragma once

#include "BandMember.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/Property.hpp"
#include "Gameplay/Shop.hpp"
#include "Gameplay/EventItem.hpp"
#include "HireItem.hpp"
#include <array>

class GameProfile
{
    VH_PROPERTY_RW(std::string, BandName);
    VH_PROPERTY_RW(int32_t, Money);
    VH_PROPERTY_RW(uint32_t, Day);
    VH_PROPERTY_RW(float, Skill);
    VH_PROPERTY_RW(float, Reputation);
    VH_PROPERTY_RW(float, Popularity);
    VH_PROPERTY_RW(float, Quality);

    VH_PROPERTY_RW(BandMember, Guitarist);
    VH_PROPERTY_RW(BandMember, Bassist);
    VH_PROPERTY_RW(BandMember, Drummer);

    VH_PROPERTY_RW(Shop, Shop);

    VH_PROPERTY_RW(std::vector<HireItem>, GuitarHires);
    VH_PROPERTY_RW(std::vector<HireItem>, BassHires);
    VH_PROPERTY_RW(std::vector<HireItem>, DrumHires);

    VH_PROPERTY_RW(std::vector<EventItem>, Events);
    VH_PROPERTY_RW(bool, ShowMOTG);

public:
    GameProfile(int slot, const char* name = "");

    /* Save profile to disc */
    bool Save() const;

    /* Load profile from disc */
    bool Load();

    static bool GetProfilePath(std::string& outPath, int slot);

    void AddMember(const BandMember& member);
    const BandMember& GetBandMember(eBandSlot::Type type) const;

    void SetItem(eBandSlot::Type type, const Item& item);

    static GameProfile* NewProfile(int slot, const std::string& name);

    /*
        Withdraws a specified amount of money from the profile,
        returns false on error (e.g. not enough money on balance)
    */
    bool WithdrawMoney(int32_t amount);

    /*
        Start a next day:
        Apply event modifiers (no event if eventId == -1)
        Generate new events, shop items, hire items,
        check for game over condition.
    */
    void StartNextDay(int eventId);

    std::vector<EventItem> GenerateEvents();

private:
    static const uint32_t VERSION_TAG;

    int mSlot;
};
