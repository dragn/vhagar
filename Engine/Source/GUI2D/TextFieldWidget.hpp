#pragma once

#include "Widget.hpp"
#include <string>
#include "Utils/Delegate.hpp"

namespace gui
{

class TextFieldWidget : public Widget
{
public:
    TextFieldWidget();
    ~TextFieldWidget();

    virtual void OnClick(int32_t x, int32_t y) override;
    virtual void Draw(int32_t x, int32_t y) override;

    virtual void OnFocus() override;
    virtual void OnBlur() override;

    virtual void HandleEvent(SDL_Event* event) override;

    void SetMaxSize(uint32_t size);
    uint32_t GetMaxSize() const;

    void SetValue(const char* value);
    const char* GetValue() const;

    vh::MultiDelegate<std::string> OnChange;

private:
    static Uint32 TimerCallback(Uint32, void*);

    std::string mContent;
    uint32_t mMaxSize;
    bool mFocus;
    bool mBlink;

    SDL_TimerID mTimer;
};

} // namespace gui
