#pragma once

#include "Modules/GUI2D/Widget.hpp"
#include "Modules/Renderer2D/Color.hpp"
#include "Modules/standard.hpp"

namespace gui
{

class GUI2D; // forward decl

class View
{
    friend class GUI2D;

public:
    View(const char* name = "NonameView");
    virtual ~View();

    void AddWidget(Widget* widget);
    void RemoveWidget(Widget* widget);

    void SetBackground(const vh::Color& color);
    void SetBackground(const char* imagePath);

    vh::MultiDelegate<> OnDestroy;

    const char* GetName() const { return mName.c_str(); }

protected:
    /* Destroy all widgets */
    void Destroy();
    virtual void Render();

private:
    Widget* mRootWidget;
    std::string mName;
};

}
