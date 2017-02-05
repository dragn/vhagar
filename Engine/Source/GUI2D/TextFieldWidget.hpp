#pragma once

#include "Widget.hpp"
#include <string>

namespace gui
{

class TextFieldWidget : public Widget
{
public:
    TextFieldWidget();

    virtual void OnClick(int32_t x, int32_t y) override;
    virtual void Draw(int32_t x, int32_t y) override;

    virtual void OnFocus() override;
    virtual void OnBlur() override;

    virtual void HandleEvent(SDL_Event* event) override;

    void SetMaxSize(uint32_t size);
    uint32_t GetMaxSize() const;

    void SetValue(const char* value);
    const char* GetValue() const;

private:
    std::string mContent;
    uint32_t mMaxSize;
    bool mFocus;
};

} // namespace gui
