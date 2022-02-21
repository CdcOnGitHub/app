#include "Label.hpp"
#include "../windows/Window.hpp"

Label::Label(std::string const& text) {
    m_type = "Label";
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoresize();
    this->show();
}

void Label::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        Graphics g(hdc);
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        RectF r;
        g.MeasureString(
            toWString(m_text).c_str(), -1,
            &Font(hdc, Manager::get()->loadFont(m_font, m_fontsize)),
            { 0, 0, 0, 0 }, &r
        );
        this->resize(static_cast<int>(r.Width) + 1_px, static_cast<int>(r.Height));
        m_autoresize = true;
    }

    Widget::updateSize(hdc, available);
}

void Label::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    StringFormat f;
    g.DrawString(
        toWString(m_text).c_str(), -1,
        &Font(hdc, Manager::get()->loadFont(m_font, m_fontsize)),
        toRectF(r),
        &f,
        &SolidBrush(m_color)
    );
    
    Widget::paint(hdc, ps);
}
