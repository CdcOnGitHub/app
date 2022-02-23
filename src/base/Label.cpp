#include "Label.hpp"
#include "../windows/Window.hpp"

Label::Label(std::string const& text) {
    m_typeName = "Label";
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Label::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        Graphics g(hdc);
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        RectF r;
        Font font(hdc, Manager::get()->loadFont(m_font, m_fontSize));
        if (m_wordWrap) {
            g.MeasureString(
                toWString(m_text).c_str(), -1,
                &font, { 
                    0, 0,
                    static_cast<REAL>(available.cx),
                    static_cast<REAL>(available.cy)
                }, &r
            );
        } else {
            g.MeasureString(
                toWString(m_text).c_str(), -1,
                &font, { 
                    0, 0, 0, 0
                }, &r
            );
        }
        if (r.Width > available.cx) r.Width = static_cast<REAL>(available.cx);
        if (r.Height > available.cy) r.Height = static_cast<REAL>(available.cy);
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
    Font font(hdc, Manager::get()->loadFont(m_font, m_fontSize));
    SolidBrush brush(m_color);
    if (m_wordWrap) {
        g.DrawString(
            toWString(m_text).c_str(), -1,
            &font, toRectF(r), &f, &brush
        );
    } else {
        Region reg(r);
        g.SetClip(&reg);
        g.DrawString(
            toWString(m_text).c_str(), -1,
            &font, toPointF(r), &f, &brush
        );
    }
    
    Widget::paint(hdc, ps);
}
