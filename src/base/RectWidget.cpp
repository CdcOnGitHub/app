#include "RectWidget.hpp"

RectWidget::RectWidget() {
    m_type = "RectWidget";
    this->color({ 255, 255, 255 });
    this->autoresize();
    this->show();
}

void RectWidget::setCornerRadius(int c) {
    m_cornerRadius = c;
    this->update();
}

void RectWidget::updateSize(HDC hdc, SIZE available) {
    Widget::updateSize(hdc, available);
    if (m_autoresize) {
        this->resize(available.cx, available.cy);
        m_autoresize = true;
    }
}

void RectWidget::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    if (m_cornerRadius) {
        FillRoundRect(
            &g, r, m_color, m_cornerRadius / 2, m_cornerRadius
        );
    } else {
        g.FillRectangle(&SolidBrush(m_color), toRectF(r));
    }
    
    Widget::paint(hdc, ps);
}



