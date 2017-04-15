#include "PCH.hpp"
#include "Shop.hpp"
#include "Gameplay/GameProfile.hpp"

void GenGuitars(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Guitar, "Guitar 1", "guitars_0002_guitar_5.png", 100, -10, 20, 10));
    list.push_back(ShopItem(eBandSlot::Guitar, "Guitar 2", "guitars_0006_guitar_3.png", 200, 40, -10, 20));
}

void GenBasses(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Bass, "Bass 1", "guitars_0004_bass_3.png", 120, 0, -20, 10));
}

void GenDrums(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Drums, "Drums 1", "drums_cheap.png", 200, -20, -10, 0));
    list.push_back(ShopItem(eBandSlot::Drums, "Drums 2", "drums_cheap.png", 250, 40, 10, 20));
}

const std::vector<ShopItem>& Shop::GetItems(eBandSlot::Type type) const
{
    switch (type)
    {
    case eBandSlot::Guitar: return mGuitars;
    case eBandSlot::Bass: return mBassGuitars;
    case eBandSlot::Drums: return mDrums;
    default:
        LOG(ERROR) << "Unsupported type: " << type;
        return mGuitars;
    }
}

void Shop::SetItems(eBandSlot::Type type, std::vector<ShopItem>& items)
{
    switch (type)
    {
    case eBandSlot::Guitar:
        mGuitars = items;
        break;
    case eBandSlot::Bass:
        mBassGuitars = items;
        break;
    case eBandSlot::Drums:
        mDrums = items;
        break;
    default:
        LOG(ERROR) << "Unsupported type: " << type;
    }
}

Shop Shop::Generate()
{
    std::vector<ShopItem> guitars;
    std::vector<ShopItem> basses;
    std::vector<ShopItem> drums;

    GenGuitars(guitars);
    GenBasses(basses);
    GenDrums(drums);

    return Shop(guitars, basses, drums);
}

bool Shop::BuyItem(GameProfile* profile, eBandSlot::Type type, int idx)
{
    Shop shop = profile->GetShop();
    std::vector<ShopItem> items = shop.GetItems(type);
    ShopItem item = items[idx];

    if (profile->WithdrawMoney(item.GetCost()))
    {
        // -- remove item from the shop
        items.erase(items.begin() + idx);
        shop.SetItems(type, items);

        // -- update profile's shop
        profile->SetShop(shop);

        // -- equip new item
        profile->SetItem(type, Item(item.GetName(), item.GetImg()));

        // -- apply modifiers
        profile->SetReputation(profile->GetReputation() + item.GetReputation());
        profile->SetPopularity(profile->GetPopularity() + item.GetPopularity());

        return true;
    }
    else
    {
        return false;
    }
}
