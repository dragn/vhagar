#pragma once

namespace eBandSlot
{
enum Type
{
    Drums = 0,
    Bass,
    Guitar,
    Vocal,
    Keyboard,
    MAX
};
}

class BandMember
{
private:
    std::string mName;
    eBandSlot::Type mSlot;
};
