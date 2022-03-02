#pragma once

#include "Widget.hpp"

class Button : public TextWidget {
public:
    using Callback = std::function<void(Button*)>;
    static int s_rounding;
    static int s_topPad;
    static int s_sidePad;

protected:
    Color m_bgColor;
    Callback m_callback;

public:
    Button(std::string const& text);

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;

    void bg(Color const&);
    Color bg() const;

    void click() override;
    void callback(Callback cb);
};
