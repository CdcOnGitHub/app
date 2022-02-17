#pragma once

#include "Widget.hpp"
#include <string>
#include <dwmapi.h>
#include <functional>

class Button : public TextWidget {
public:
    using Callback = std::function<void(Button*)>;

protected:
    std::string m_text;
    HMENU m_id;
    Callback m_callback;

    void updateParent() override;
    void paint(DRAWITEMSTRUCT*) override;

public:
    Button(std::string const& text);
    virtual ~Button();

    void click();
    void setCallback(Callback cb);

    static Button* fromHMENU(HMENU menu);

    LRESULT paint(NMCUSTOMDRAW*);

    static LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
};
