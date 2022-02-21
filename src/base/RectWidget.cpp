#include "RectWidget.hpp"

RectWidget::RectWidget() {
    m_type = "RectWidget";
    this->color(RGB(255, 255, 255));
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
    if (m_cornerRadius) {
        auto old = SelectObject(hdc, this->brush(m_color));
        RoundRect(hdc, r.left, r.top, r.right, r.bottom, m_cornerRadius, m_cornerRadius);
        SelectObject(hdc, old);
    } else {
        FillRect(hdc, &r, this->brush(m_color));
    }
    Widget::paint(hdc, ps);
}



