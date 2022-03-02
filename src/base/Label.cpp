#include "Label.hpp"
#include "../windows/Window.hpp"

Label::Label(std::string const& text) {
    m_typeName = "Label";
    this->text(text);
    this->font(Style::font());
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Label::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        auto r = this->measureText(hdc, available);
        this->resize(static_cast<int>(r.Width) + 1_px, static_cast<int>(r.Height));
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Label::paint(HDC hdc, PAINTSTRUCT* ps) {
    TextWidget::paint(hdc, ps);
}
