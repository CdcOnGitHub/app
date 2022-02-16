#include "Button.hpp"

Button::Button(std::string const& text) {
    m_hwnd = CreateWindowA(
        "Button", "",
        SS_LEFT,
        m_x, m_y,
        m_width, m_height,
        nullptr,
        nullptr,
        Manager::get()->getInst(),
        nullptr 
    );
    if (!m_hwnd) {
        throw std::runtime_error("Unable to create Button");
    }
    this->setText(text);
    this->show();
}

void Button::setText(std::string const& text) {
    this->m_text = text;
    SetWindowTextA(m_hwnd, text.c_str());
}

void Button::setFont(std::string const& font, int size) {
    SendMessage(m_hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(Manager::get()->loadFont(font, size)), true);
}
