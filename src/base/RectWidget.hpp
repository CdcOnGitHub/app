#pragma once

#include "Widget.hpp"

class RectWidget : public ColorWidget {
protected:
    int m_cornerRadius = 0;

public:
    RectWidget();

    void setCornerRadius(int c);
    void paint(HDC, PAINTSTRUCT*) override;
};
