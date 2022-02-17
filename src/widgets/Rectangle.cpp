#include "Rectangle.hpp"

RectWidget::RectWidget() {
    m_hwnd = CreateWindowA(
        "Static", "",
        SS_OWNERDRAW,
        m_x, m_y,
        m_width, m_height,
        nullptr,
        nullptr,
        Manager::get()->getInst(),
        nullptr 
    );
    if (!m_hwnd) {
        throw std::runtime_error("Unable to create RectWidget");
    }
    this->init(m_hwnd);
    this->setColor(RGB(255, 255, 255));
    this->show();
}

RectWidget::~RectWidget() {
    DeleteObject(m_brush);
}

void RectWidget::paint(DRAWITEMSTRUCT* item) {
    if (!m_brush) m_brush = CreateSolidBrush(m_color);
    FillRect(item->hDC, &item->rcItem, m_brush);
}

void RectWidget::updateParent() {
    SetWindowLongPtrA(m_hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW);
}


