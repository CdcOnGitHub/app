#include "Button.hpp"

int Button::s_rounding = 3_px;

Button::Button(std::string const& text) {
    m_typeName = "Button";
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->bg(Color(120, 120, 120));
    this->autoResize();
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
        auto r = this->measureText(hdc, available);
        this->resize(static_cast<int>(r.Width) + 30_px, static_cast<int>(r.Height) + 10_px);
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Button::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    InitGraphics(g);

    FillRoundRect(&g, r, 
        m_mousedown ?
            color::darken(m_bgColor, 50) :
        (m_hovered ?
            color::lighten(m_bgColor, 50) :
            m_bgColor
        ),
        s_rounding / 2, s_rounding
    );
    
    StringFormat f;
    f.SetAlignment(StringAlignmentCenter);
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    this->paintText(hdc, r, f);

    Widget::paint(hdc, ps);
}
