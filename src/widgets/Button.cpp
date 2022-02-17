#include "Button.hpp"

static std::unordered_map<HMENU, Button*> g_hmenus;

Button::Button(std::string const& text) {
    m_id = Manager::get()->acquireMenuID();
    m_hwnd = CreateWindowA(
        "Button", "",
        SS_LEFT | BS_PUSHBUTTON,
        m_x, m_y,
        m_width, m_height,
        nullptr, nullptr,
        nullptr, nullptr 
    );
    if (!m_hwnd) {
        throw std::runtime_error("Unable to create Button");
    }
    g_hmenus[m_id] = this;
    std::cout << "created button with id " << m_id << "\n";
    this->init(m_hwnd);
    this->setText(text);
    this->setFont("Segoe UI");
    this->autoresize();
    this->show();
}

Button::~Button() {
    g_hmenus.erase(m_id);
    Manager::get()->relinquishMenuID(m_id);
}

Button* Button::fromHMENU(HMENU menu) {
    if (!g_hmenus.count(menu))
        return nullptr;
    return g_hmenus.at(menu);
}

void Button::click() {
    m_callback(this);
}

void Button::setCallback(Callback cb) {
    m_callback = cb;
}

void Button::updateParent() {
    SetWindowLongPtrA(m_hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
    SetWindowLongPtrA(m_hwnd, GWLP_ID, reinterpret_cast<LONG_PTR>(m_id));
}

void Button::paint(DRAWITEMSTRUCT* item) {
    if (m_autoresize) {
        SIZE size;
        GetTextExtentPoint32A(item->hDC, m_text.c_str(), static_cast<int>(m_text.size()), &size);
        this->resize(size.cx, size.cy);
        m_autoresize = true;
    }
    SetBkMode(item->hDC, TRANSPARENT);
    SetTextColor(item->hDC, m_color);
    DrawTextA(item->hDC, m_text.c_str(), -1, &item->rcItem, DT_CENTER | DT_VCENTER);
}

LRESULT Button::paint(NMCUSTOMDRAW* item) {
    static auto selectbrush = CreateSolidBrush(RGB(180, 0, 0));

    std::cout << "Button::paint" << "\n";

    HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

    HGDIOBJ old_pen = SelectObject(item->hdc, pen);
    HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

    RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);
    
    SelectObject(item->hdc, old_pen);
    SelectObject(item->hdc, old_brush);
    DeleteObject(pen);
    
    return CDRF_DODEFAULT;
}

LRESULT Button::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_NOTIFY: {
            LPNMHDR item = reinterpret_cast<LPNMHDR>(lp);
            if (item->code == NM_CUSTOMDRAW) {
                auto widget = dynamic_cast<Button*>(Widget::fromHWND(item->hwndFrom));
                auto customdraw = reinterpret_cast<NMCUSTOMDRAW*>(item);
                if (widget) return widget->paint(customdraw);
            }
        } break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}
