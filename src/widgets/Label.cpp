#include "Label.hpp"
#include "../windows/Window.hpp"

Label::Label(std::string const& text) {
    m_hwnd = CreateWindowA(
        "Static", "",
        SS_LEFT | SS_OWNERDRAW,
        m_x, m_y,
        m_width, m_height,
        nullptr,
        nullptr,
        Manager::get()->getInst(),
        nullptr 
    );
    if (!m_hwnd) {
        throw std::runtime_error("Unable to create Label");
    }
    this->init(m_hwnd);
    this->setText(text);
    this->setFont("Segoe UI");
    this->setColor(RGB(255, 255, 255));
    this->autoresize();
    this->show();
}

void Label::paint(DRAWITEMSTRUCT* item) {
    if (m_autoresize) {
        SIZE size;
        GetTextExtentPoint32A(item->hDC, m_text.c_str(), static_cast<int>(m_text.size()), &size);
        this->resize(size.cx, size.cy);
        m_autoresize = true;
    }
    SetBkMode(item->hDC, TRANSPARENT);
    SetTextColor(item->hDC, m_color);
    DrawTextA(item->hDC, m_text.c_str(), -1, &item->rcItem, DT_LEFT);
}

void Label::updateParent() {
    SetWindowLongPtrA(m_hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW);
}
