#include "Checkbox.hpp"
#include "Button.hpp"

Checkbox::Checkbox(std::string const& text, bool checked) {
    m_type = "Checkbox";
    this->check(checked);
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoresize();
    this->show();
}

void Checkbox::check(bool check) {
    m_checked = check;
    this->update();
}

void Checkbox::uncheck() {
    m_checked = false;
    this->update();
}

void Checkbox::toggle() {
    m_checked = !m_checked;
    this->update();
}

bool Checkbox::checked() const {
    return m_checked;
}

void Checkbox::click() {
    this->toggle();
    if (m_callback) {
        m_callback(this);
    }
}

void Checkbox::callback(Callback cb) {
    m_callback = cb;
}

bool Checkbox::wantsMouse() const {
    return true;
}

HCURSOR Checkbox::cursor() const {
    return Manager::cursor(IDC_HAND);
}

void Checkbox::updateSize(HDC hdc, SIZE available) {
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
        this->resize(static_cast<int>(r.Width) + 40_px, static_cast<int>(r.Height));
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Checkbox::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Rect cr;
    cr.X = r.X;
    cr.Y = r.Y + (r.Height - m_fontsize) / 2;
    cr.Width = m_fontsize;
    cr.Height = m_fontsize;

    r.X += m_fontsize + 5_px;
    r.Width -= m_fontsize - 5_px;

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    auto color = m_checked ? Style::primary() : Style::secondary();
    FillRoundRect(&g, cr, 
        m_mousedown ?
            color::darken(color, 50) :
        (m_hovered ?
            color::lighten(color, 50) :
            color
        ),
        Button::s_rounding, Button::s_rounding * 2
    );

    if (m_checked) {
        constexpr const auto p = 5;
        constexpr const auto t = 3;
        auto w = cr.Height - 2 * p;
        auto wt = w / t;
        auto wh = (t - 1) * wt;
        auto d = (cr.Height - wh) / 2;
        Pen pen(Style::text(), m_fontsize / 7.f);
        GraphicsPath path;
        path.AddLine(
            cr.X + p, cr.Y + d + (t - 2) * wt,
            cr.X + p + wt, cr.Y + d + wh
        );
        path.AddLine(
            cr.X + p + wt, cr.Y + d + wh,
            cr.X + p + w, cr.Y + d
        );
        g.DrawPath(&pen, &path);
    }
    
    StringFormat f;
    f.SetAlignment(StringAlignmentNear);
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
