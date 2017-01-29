#pragma once

#include "App/Component.hpp"
#include "View.hpp"

namespace gui
{

class GUI2D : public vh::Component
{
    COMPONENT_NAME(GUI2D);

public:
    GUI2D()
        : vh::Component(vh::eTickFrequency::NORMAL)
        , mActiveView(nullptr)
    {}

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    void SetView(View* view);

private:
    View* mActiveView;
};

}
