#pragma once

#include "Widget.hpp"
#include <string>

class Label : public TextWidget {
public:
    Label(std::string const& text);

    void updateSize(HDC hdc, SIZE size) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};
