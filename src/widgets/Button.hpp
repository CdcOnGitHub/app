#pragma once

#include "Widget.hpp"
#include <string>

class Button : public Widget {
protected:
    std::string m_text;

public:
    Button(std::string const& text);

    void setText(std::string const& text);
    void setFont(std::string const& font, int size = 0);
};
