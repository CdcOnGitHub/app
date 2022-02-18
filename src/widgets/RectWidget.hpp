#pragma once

#include "Widget.hpp"

class RectWidget : public ColorWidget {
protected:
    HBRUSH m_brush = nullptr;
    int m_cornerRadius = 0;

public:
    RectWidget();
    virtual ~RectWidget();

    void setCornerRadius(int c);

    void paint(HDC, PAINTSTRUCT*) override;
};
