#include "Checkbox.hpp"
#include "Button.hpp"

Checkbox::Checkbox(std::string const& text, bool checked) {
    m_typeName = "Checkbox";
    this->check(checked);
    this->text(text);
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoResize();
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
        auto r = this->measureText(hdc, available);
        this->resize(static_cast<int>(r.Width) + m_fontSize + 5_px, static_cast<int>(r.Height));
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Checkbox::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Rect cr;
    cr.X = r.X;
    cr.Y = r.Y + (r.Height - m_fontSize) / 2;
    cr.Width = m_fontSize;
    cr.Height = m_fontSize;

    r.X += m_fontSize + 5_px;
    r.Width -= m_fontSize - 5_px;

    Graphics g(hdc);
    InitGraphics(g);

    auto color = m_checked ? Style::primary() : Style::dark();
    FillRoundRect(&g, cr, 
        m_mousedown ?
            color::darken(color, 50) :
        (m_hovered ?
            color::lighten(color, 50) :
            color
        ),
        Button::s_rounding / 2, Button::s_rounding
    );

    if (m_checked) {
        const auto p = 4_px;
        const auto t = 3;
        auto w = cr.Height - 2 * p;
        auto wt = w / t;
        auto wh = (t - 1) * wt;
        auto d = (cr.Height - wh) / 2;
        Pen pen(Style::text(), m_fontSize / 7.f);
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
    this->paintText(hdc, r, f);

    Widget::paint(hdc, ps);
}
