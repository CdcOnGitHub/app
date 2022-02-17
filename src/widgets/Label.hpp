#pragma once

#include "Widget.hpp"
#include <string>

class Label : public TextWidget {
protected:
    void updateParent() override;

public:
    Label(std::string const& text);

    void paint(DRAWITEMSTRUCT*) override;
};
