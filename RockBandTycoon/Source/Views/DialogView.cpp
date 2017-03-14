#include "PCH.hpp"
#include "DialogView.hpp"
#include "Widgets/PanelWidget.hpp"

using namespace vh;
using namespace gui;

const static int32_t MARGIN = 8;

DialogView::DialogView(const std::string& text)
{
    mPanelWdg = new PanelWidget();
    mPanelWdg->SetSize(200, 100);
    mPanelWdg->SetPos(ePos::Center, ePos::Center, eAnchor::CenterCenter);
    AddWidget(mPanelWdg);

    mTextWdg = new TextWidget(text.c_str());
    mTextWdg->SetColor(Colors::Orange);
    mTextWdg->SetPos(MARGIN, MARGIN);
    mTextWdg->SetWrap(200 - MARGIN * 2);
    mPanelWdg->AddChild(mTextWdg);
}

void DialogView::AddOptionWidget(uint32_t idx, Option& option)
{
    mTextWdg->UpdateSize();
    int32_t w, h; mTextWdg->GetSize(w, h);

    ButtonWidget* btn = new ButtonWidget(option.text.c_str());
    btn->OnClick.Add(this, &DialogView::OnOptionSelect, idx);
    btn->SetPos(MARGIN, h + MARGIN * 2 + 20 * idx);
    btn->SetSize(200 - MARGIN * 2, 18);
    btn->SetTextColor(Colors::Blue);
    btn->SetBackground(Colors::Transparent);
    mPanelWdg->AddChild(btn);
}

void DialogView::OnOptionSelect(uint32_t idx)
{
    if (idx < mOptions.size())
    {
        CHECK(mOptions[idx]);
        mOptions[idx]->OnSelect();
    }
}

vh::Delegate& DialogView::AddOption(const std::string& text)
{
    mOptions.push_back(new Option());
    mOptions.back()->text = text;
    AddOptionWidget(mOptions.size() - 1, *mOptions.back());
    return mOptions.back()->OnSelect;
}

DialogView::~DialogView()
{
    for (Option* opt : mOptions)
    {
        SafeDelete(opt);
    }
}
