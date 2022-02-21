#include "Checkbox.hpp"

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
        g.MeasureString(
            toWString(m_text).c_str(), -1,
            &Font(hdc, Manager::get()->loadFont(m_font, m_fontsize)),
            { 0, 0, 0, 0 }, &r
        );
        this->resize(static_cast<int>(r.Width) + 40_px, static_cast<int>(r.Height));
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Checkbox::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Rect cr;
    cr.X = r.X;
    cr.Y = r.Y;
    cr.Width = r.Height;
    cr.Height = r.Height;

    r.X += 40_px;
    r.Width -= 40_px;

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    auto color = m_checked ? Style::primary() : Style::secondary();
    FillRoundRect(&g, cr, 
        m_mousedown ?
            color_utils::darken(color, 50) :
        (m_hovered ?
            color_utils::lighten(color, 50) :
            color
        ),
        5_px, 10_px
    );

    if (m_checked) {
        constexpr const auto p = 5;
        constexpr const auto t = 3;
        auto w = r.Height - 2 * p;
        auto wt = w / t;
        auto wh = (t - 1) * wt;
        auto d = (r.Height - wh) / 2;
        g.DrawLine(
            &Pen(Style::text(), 3.0_pxf),
            cr.X + p, cr.Y + d + (t - 2) * wt,
            cr.X + p + wt, cr.Y + d + wh
        );
        g.DrawLine(
            &Pen(Style::text(), 3.0_pxf),
            cr.X + p + wt, cr.Y + d + wh,
            cr.X + p + w, cr.Y + d
        );
    }
    
    StringFormat f;
    f.SetAlignment(StringAlignmentNear);
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    g.DrawString(
        toWString(m_text).c_str(), -1,
        &Font(hdc, Manager::get()->loadFont(m_font, m_fontsize)),
        toRectF(r),
        &f, &SolidBrush(m_color)
    );

    Widget::paint(hdc, ps);
}
