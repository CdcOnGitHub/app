#pragma once

#include "Widget.hpp"
#include <string>

class Label : public TextWidget {
public:
    Label(std::string const& text);

    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};
