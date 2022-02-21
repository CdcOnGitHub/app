#include "Label.hpp"
#include "../windows/Window.hpp"

Label::Label(std::string const& text) {
    m_type = "Label";
    this->text(text);
    this->setFont("Segoe UI");
    this->color(Style::text());
    this->autoresize();
    this->show();
}

void Label::updateSize(HDC hdc, SIZE available) {
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));
    if (m_autoresize) {
        SIZE size;
        GetTextExtentPoint32A(hdc, m_text.c_str(), static_cast<int>(m_text.size()), &size);
        this->resize(size.cx, size.cy);
        m_autoresize = true;
    }
    SelectObject(hdc, oldFont);

    Widget::updateSize(hdc, available);
}

void Label::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));
    auto rect = this->rect();
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_color);
    DrawTextA(hdc, m_text.c_str(), -1, &rect, DT_LEFT);
    SelectObject(hdc, oldFont);
    
    Widget::paint(hdc, ps);
}
