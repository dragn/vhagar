#pragma once

#include "ShopItem.hpp"

class Shop
{
    VH_PROPERTY_RW(std::vector<ShopItem>, Guitars);
    VH_PROPERTY_RW(std::vector<ShopItem>, BassGuitars);
    VH_PROPERTY_RW(std::vector<ShopItem>, Drums);

public:
    Shop() {}
    Shop(const std::vector<ShopItem> guitars, const std::vector<ShopItem> bass, const std::vector<ShopItem> drums)
        : mGuitars(guitars), mBassGuitars(bass), mDrums(drums)
    { }

    const std::vector<ShopItem>& GetItems(eBandSlot::Type type) const;
    void SetItems(eBandSlot::Type type, std::vector<ShopItem>& items);

    static Shop Generate();
};
