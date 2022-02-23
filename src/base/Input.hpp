#pragma once

#include "Widget.hpp"
#include <string>

class Input : public TextWidget {
protected:
    std::string m_placeHolder = "";

public:
    Input();

    bool wantsMouse() const override;
    HCURSOR cursor() const;
    void click() override;

    void placeHolder(std::string const& text);
    
    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};

