#pragma once

class Item
{
public:
    Item() {}
    Item(const std::string& name, const std::string& img)
        : mName(name)
        , mImg(img)
    {}

private:
    std::string mName;
    std::string mImg;
};
