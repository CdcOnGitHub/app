#include "Button.hpp"

int Button::s_rounding = 4_px;
int Button::s_topPad   = 4_px;
int Button::s_sidePad  = 20_px;

Button::Button(std::string const& text) {
    m_typeName = "Button";
    this->text(text);
    this->font(Style::font());
    this->color(Style::text());
    this->bg(Style::button());
    this->autoResize();
    this->show();
}

void Button::click() {
    if (m_callback) {
        m_callback(this);
    }
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
        this->resize(static_cast<int>(r.Width) + s_sidePad * 2, static_cast<int>(r.Height) + s_topPad * 2);
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Button::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    InitGraphics(g);

    auto c1 = m_mousedown ?
            color::lighten(m_bgColor, Style::buttonPress()) :
        (m_hovered ?
            color::lighten(m_bgColor, 50) :
            m_bgColor
        );
    auto c2 = color::darken(c1, Style::buttonGradient());

    FillRoundRect(
        &g, r,
        LinearGradientBrush(
            Point { 0, r.Y }, Point { 0, r.Y + r.Height }, c1, c2
        ),
        s_rounding / 2, s_rounding
    );
    
    StringFormat f;
    f.SetAlignment(StringAlignmentCenter);
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    this->paintText(hdc, r, f);

    if (Style::useBorders()) {
        DrawRoundRect(
            &g, r,
            color::lighten(c1, Style::buttonBorder()),
            s_rounding / 2, s_rounding
        );
    }

    Widget::paint(hdc, ps);
}
