#pragma once

class Item
{
public:
    Item() {}
    Item(const std::string& name, const std::string& img)
        : mName(name)
        , mImg(img)
    {}

    const std::string& GetName() const { return mName; }
    const std::string& GetImg() const { return mImg; }

private:
    std::string mName;
    std::string mImg;
};
