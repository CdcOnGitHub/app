#include "Button.hpp"

int Button::s_rounding = 2_px;

Button::Button(std::string const& text) {
    m_type = "Button";
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->bg(Color(120, 120, 120));
    this->autoresize();
    this->show();
}

void Button::click() {
    m_callback(this);
}

void Button::callback(Callback cb) {
    m_callback = cb;
}

void Button::bg(Color const& c) {
    m_bgColor = c;
    this->update();
}

Color Button::bg() const {
    return m_bgColor;
}

bool Button::wantsMouse() const {
    return true;
}

HCURSOR Button::cursor() const {
    return Manager::cursor(IDC_HAND);
}

void Button::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        Graphics g(hdc);
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        RectF r;
        Font font(hdc, Manager::get()->loadFont(m_font, m_fontsize));
        g.MeasureString(
            toWString(m_text).c_str(), -1,
            &font, { 0, 0, static_cast<REAL>(available.cx), static_cast<REAL>(available.cy) },
            &r
        );
        if (r.Width > available.cx) r.Width = static_cast<REAL>(available.cx);
        if (r.Height > available.cy) r.Height = static_cast<REAL>(available.cy);
        this->resize(static_cast<int>(r.Width) + 30_px, static_cast<int>(r.Height) + 10_px);
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Button::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    FillRoundRect(&g, r, 
        m_mousedown ?
            color::darken(m_bgColor, 50) :
        (m_hovered ?
            color::lighten(m_bgColor, 50) :
            m_bgColor
        ),
        s_rounding, s_rounding * 2
    );

    StringFormat f;
    f.SetAlignment(StringAlignmentCenter);
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    Font font(hdc, Manager::get()->loadFont(m_font, m_fontsize));
    SolidBrush brush(m_color);
    g.DrawString(
        toWString(m_text).c_str(), -1,
        &font, toRectF(r), &f, &brush
    );

    Widget::paint(hdc, ps);
}
