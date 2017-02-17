#pragma once

#include "GUI2D/View.hpp"
#include "GUI2D/TextFieldWidget.hpp"
#include "Widgets/BandMemberWidget.hpp"

class NewGameView : public gui::View
{
public:
    NewGameView(int slot);

    void OnStart();

private:
    gui::TextFieldWidget* mBandNameFld;
    gui::TextFieldWidget* mYourNameFld;

    BandMemberWidget* mBandMemberWdg;

    std::string GetRandomName();

    void GenBandMember();

    int mSlot;
};
