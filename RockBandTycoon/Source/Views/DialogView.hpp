#pragma once

#include "GUI2D/View.hpp"
#include "Widgets/DialogWidget.hpp"
#include "Widgets/PanelWidget.hpp"
#include "GUI2D/TextWidget.hpp"
#include "Utils/Delegate.hpp"

class DialogView : public gui::View
{
    struct Option
    {
        std::string text;
        vh::Delegate OnSelect;
    };

public:
    DialogView(const std::string& text);
    virtual ~DialogView();
    vh::Delegate& AddOption(const std::string& text);

private:
    void AddOptionWidget(uint32_t idx, Option& option);
    void OnOptionSelect(uint32_t idx);

    PanelWidget* mPanelWdg;
    gui::TextWidget* mTextWdg;

    std::vector<Option*> mOptions;
};
