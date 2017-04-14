#include "PCH.hpp"
#include "Shop.hpp"

void GenGuitars(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Guitar, "Guitar 1", "guitars_0002_guitar_5.png", 100));
    list.push_back(ShopItem(eBandSlot::Guitar, "Guitar 2", "guitars_0006_guitar_3.png", 200));
}

void GenBasses(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Bass, "Bass 1", "guitars_0004_bass_3.png", 120));
}

void GenDrums(std::vector<ShopItem>& list)
{
    list.push_back(ShopItem(eBandSlot::Drums, "Drums 1", "drums_cheap.png", 200));
    list.push_back(ShopItem(eBandSlot::Drums, "Drums 2", "drums_cheap.png", 250));
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
