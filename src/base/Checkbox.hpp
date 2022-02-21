#pragma once

#include "Widget.hpp"

class Checkbox : public TextWidget {
public:
    using Callback = std::function<void(Checkbox*)>;

protected:
    Callback m_callback;
    bool m_checked = false;

public:
    Checkbox(std::string const& text, bool checked = false);

    void updateSize(HDC hdc, SIZE available) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;

    void check(bool check = true);
    void uncheck();
    void toggle();
    bool checked() const;

    void click() override;
    void setCallback(Callback cb);
};
