#include "RectWidget.hpp"

RectWidget::RectWidget() {
    this->setColor(RGB(255, 255, 255));
    this->show();
}

RectWidget::~RectWidget() {
    DeleteObject(m_brush);
}

void RectWidget::setCornerRadius(int c) {
    m_cornerRadius = c;
    this->update();
}

void RectWidget::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (!m_brush) m_brush = CreateSolidBrush(m_color);
    auto r = this->rect();
    if (m_cornerRadius) {
        auto old = SelectObject(hdc, m_brush);
        RoundRect(hdc, r.left, r.top, r.right, r.bottom, m_cornerRadius, m_cornerRadius);
        SelectObject(hdc, old);
    } else {
        FillRect(hdc, &r, m_brush);
    }
    
    Widget::paint(hdc, ps);
}



