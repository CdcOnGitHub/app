#include "Label.hpp"

Label::Label(std::string const& text) {
    m_hwnd = CreateWindowA(
        "Static", "",
        SS_LEFT,
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
    this->setText(text);
    this->show();
}

void Label::setText(std::string const& text) {
    this->m_text = text;
    SetWindowTextA(m_hwnd, text.c_str());
}

void Label::setFont(std::string const& font) {
    SendMessage(m_hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(Manager::get()->loadFont(font)), true);
}

LRESULT Label::proc(UINT msg, WPARAM wp, LPARAM lp) {
    static HBRUSH hBrush = CreateSolidBrush(RGB(230,230,230));

    if (msg == WM_CTLCOLORSTATIC) {
        HDC hdcStatic = (HDC)wp;
        SetTextColor(hdcStatic, RGB(0,0,0));
        SetBkColor(hdcStatic, RGB(230,230,230));
        return (INT_PTR)hBrush;
    }
    return 0;
}
