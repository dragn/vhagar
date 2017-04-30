#pragma once

#include "DialogView.hpp"

class WaitDialogView : public DialogView
{
public:
    WaitDialogView(const std::string& text, bool* finished)
        : DialogView(text), mFinished(finished)
    {
        *mFinished = false;
    }
    ~WaitDialogView()
    {
        *mFinished = true;
    }

private:
    bool* mFinished;
};
