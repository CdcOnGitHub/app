#include "Button.hpp"

Button::Button(std::string const& text) {
    this->setText(text);
    this->setFont("Segoe UI");
    this->setColor(RGB(255, 255, 255));
    this->setBG(RGB(120, 120, 120));
    this->autoresize();
    this->show();
}

Button::~Button() {
}

void Button::click() {
    m_callback(this);
}

void Button::setCallback(Callback cb) {
    m_callback = cb;
}

void Button::setBG(COLORREF c) {
    m_bgColor = c;
    this->update();
}

COLORREF Button::getBG() const {
    return m_bgColor;
}

void Button::enter() {
    this->update();
}

void Button::leave() {
    this->update();
}

bool Button::wantsMouse() const {
    return true;
}

HCURSOR Button::cursor() const {
    static auto hand = LoadCursor(nullptr, IDC_HAND);
    return hand;
}

void Button::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (!m_bgBrush) m_bgBrush = CreateSolidBrush(m_bgColor);
    if (!m_hoverBrush) m_hoverBrush = CreateSolidBrush(color_utils::lighten(m_bgColor, 50));
    if (!m_clickBrush) m_clickBrush = CreateSolidBrush(color_utils::darken(m_bgColor, 50));
    if (!m_pen) m_pen = CreatePen(PS_SOLID, 1, m_bgColor);
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));

    if (m_autoresize) {
        SIZE size;
        GetTextExtentPoint32A(hdc, m_text.c_str(), static_cast<int>(m_text.size()), &size);
        this->resize(size.cx + 40, size.cy + 15);
        m_autoresize = true;
    }

    auto r = this->rect();

    auto oldPen = SelectObject(hdc, m_pen);
    auto oldBrush = SelectObject(hdc, m_mousedown ? m_clickBrush : (m_hovered ? m_hoverBrush : m_bgBrush));
    RoundRect(hdc, r.left, r.top, r.right, r.bottom, 10, 10);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, m_color);
    DrawTextA(hdc, m_text.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);

    Widget::paint(hdc, ps);
}
