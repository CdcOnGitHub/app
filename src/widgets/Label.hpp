#pragma once

#include "Widget.hpp"
#include <string>

class Label : public Widget {
protected:
    std::string m_text;

public:
    Label(std::string const& text);

    LRESULT proc(UINT msg, WPARAM wp, LPARAM lp) override;

    void setText(std::string const& text);
    void setFont(std::string const& font);
};
