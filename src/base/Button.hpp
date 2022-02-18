#pragma once

#include "Widget.hpp"
#include <string>
#include <dwmapi.h>
#include <functional>

class Button : public TextWidget {
public:
    using Callback = std::function<void(Button*)>;

protected:
    COLORREF m_bgColor;
    HMENU m_id;
    Callback m_callback;
    HBRUSH m_bgBrush = nullptr;
    HBRUSH m_hoverBrush = nullptr;
    HBRUSH m_clickBrush = nullptr;
    HPEN m_pen = nullptr;

    void paint(HDC hdc, PAINTSTRUCT* ps) override;

public:
    Button(std::string const& text);
    virtual ~Button();

    bool wantsMouse() const override;

    void enter() override;
    void leave() override;
    HCURSOR cursor() const;

    void setBG(COLORREF);
    COLORREF getBG() const;

    void click() override;
    void setCallback(Callback cb);
};
