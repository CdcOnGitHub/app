#include "Input.hpp"

Input::Input() {
    this->text("");
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Input::placeHolder(std::string const& text) {
    m_placeHolder = text;
    this->update();
}

bool Input::wantsMouse() const {
    return true;
}

HCURSOR Input::cursor() const {
    return Manager::cursor(IDC_IBEAM);
}

void Input::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (!m_text.size()) {
        auto oldAlpha = m_color.GetAlpha();
        auto oldStyle = m_style;
        m_style |= FontStyleItalic;
        m_color = Style::alpha(m_color, 50);
        this->paintText(hdc, this->rect());
        m_color = Style::alpha(m_color, oldAlpha);
        m_style = oldStyle;
    }
    TextWidget::paint(hdc, ps);
}

void Input::click() {}
