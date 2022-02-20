#include "Checkbox.hpp"

Checkbox::Checkbox(std::string const& text, bool checked) {
    this->check(checked);
    this->setText(text);
    this->setFont("Segoe UI");
    this->setColor(Style::text());
    this->autoresize();
    this->show();
}

Checkbox::~Checkbox() {
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
    m_callback(this);
}

void Checkbox::setCallback(Callback cb) {
    m_callback = cb;
}

void Checkbox::enter() {
    this->update();
}

void Checkbox::leave() {
    this->update();
}

bool Checkbox::wantsMouse() const {
    return true;
}

HCURSOR Checkbox::cursor() const {
    static auto hand = LoadCursor(nullptr, IDC_HAND);
    return hand;
}

void Checkbox::updateSize(HDC hdc, SIZE available) {
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));
    if (m_autoresize) {
        SIZE size;
        GetTextExtentPoint32A(hdc, m_text.c_str(), static_cast<int>(m_text.size()), &size);
        this->resize(size.cx + 40, size.cy);
        m_autoresize = true;
    }
    SelectObject(hdc, oldFont);
    Widget::updateSize(hdc, available);
}

void Checkbox::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));

    auto r = this->rect();
    auto h = r.bottom - r.top;

    RECT cr;
    cr.left = r.left;
    cr.right = r.left + h;
    cr.top = r.top;
    cr.bottom = r.bottom;

    auto color = m_checked ? Style::primary() : Style::secondary();
    auto oldPen = SelectObject(hdc, this->pen(color));
    auto oldBrush = SelectObject(hdc,
        m_mousedown ?
            this->brush(color_utils::darken(color, 50)) :
        (m_hovered ?
            this->brush(color_utils::lighten(color, 50)) :
            this->brush(color)
        )
    );
    RoundRect(hdc, cr.left, cr.top, cr.right, cr.bottom, 10, 10);
    SelectObject(hdc, oldBrush);

    if (m_checked) {
        constexpr const auto p = 5;
        constexpr const auto t = 3;
        auto w = h - 2 * p;
        auto wt = w / t;
        auto wh = (t - 1) * wt;
        auto d = (h - wh) / 2;
        SelectObject(hdc, this->pen(Style::text(), 3));
        MoveToEx(hdc, cr.left + p, cr.top + d + (t - 2) * wt, nullptr);
        LineTo(hdc, cr.left + p + wt, cr.top + d + wh);
        LineTo(hdc, cr.left + p + w, cr.top + d);
    }
    SelectObject(hdc, oldPen);

    r.left += 40;

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_color);
    DrawTextA(hdc, m_text.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);

    Widget::paint(hdc, ps);
}
