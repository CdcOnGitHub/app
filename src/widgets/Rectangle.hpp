#pragma once

#include "Widget.hpp"

class RectWidget : public ColorWidget {
protected:
    HBRUSH m_brush = nullptr;

    void updateParent() override;

public:
    RectWidget();
    virtual ~RectWidget();

    void paint(DRAWITEMSTRUCT*) override;
};
