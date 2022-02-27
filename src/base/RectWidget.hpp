#pragma once

#include "Widget.hpp"

class RectWidget : public ColorWidget {
protected:
    int m_cornerRadius = 0;
    bool m_fill = false;

public:
    RectWidget();

    void fill(bool = true);
    void cornerRadius(int c);
    void updateSize(HDC, SIZE) override;
    void paint(HDC, PAINTSTRUCT*) override;
};
