#pragma once

#include "Widget.hpp"

class Button : public TextWidget {
public:
    using Callback = std::function<void(Button*)>;

protected:
    COLORREF m_bgColor;
    Callback m_callback;

public:
    Button(std::string const& text);

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;

    void bg(COLORREF);
    COLORREF getBG() const;

    void click() override;
    void callback(Callback cb);
};
