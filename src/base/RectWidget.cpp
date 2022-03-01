#include "RectWidget.hpp"

RectWidget::RectWidget() {
    m_typeName = "RectWidget";
    this->color({ 255, 255, 255 });
    this->autoResize();
    this->show();
}

void RectWidget::fill(bool b) {
    m_fill = b;
    this->update();
}

void RectWidget::cornerRadius(int c) {
    m_cornerRadius = c;
    this->update();
}

void RectWidget::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        if (m_fill) {
            this->resize(available.cx, available.cy);
            Widget::updateSize(hdc, available);
        } else {
            Widget::updateSize(hdc, available);
            SIZE size = { 0, 0 };
            for (auto& child : m_children) {
                if (child->x() + child->width() > size.cx) {
                    size.cx = child->x() + child->width();
                }
                if (child->y() + child->height() > size.cy) {
                    size.cy = child->y() + child->height();
                }
            }
            this->resize(size.cx, size.cy);
        }
        m_autoresize = true;
    } else {
        Widget::updateSize(hdc, available);
    }
}

void RectWidget::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    InitGraphics(g);

    if (m_cornerRadius) {
        FillRoundRect(
            &g, r, m_color, m_cornerRadius / 2, m_cornerRadius
        );
    } else {
        SolidBrush brush(m_color);
        g.FillRectangle(&brush, toRectF(r));
    }
    
    Widget::paint(hdc, ps);
}



