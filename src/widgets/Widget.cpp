#include "Widget.hpp"
#include <Window.hpp>

static std::unordered_map<HWND, Widget*> g_hwnds;

void Widget::updateParent() {
    SetWindowLongPtrA(m_hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
}

void Widget::add(Widget* child) {
    if (!child->m_parent) {
        child->m_parent = this;
        if (this->m_window) {
            SetParent(child->m_hwnd, this->m_window->m_hwnd);
        } else {
            SetParent(child->m_hwnd, this->m_hwnd);
            child->m_window = dynamic_cast<Window*>(this);
        }
        child->updateParent();
        child->updatePosition();
        this->m_children.push_back(child);
    }
}

void Widget::remove(Widget* child, bool release) {
    for (auto& widget : m_children) {
        if (child == widget) {
            child->m_parent = nullptr;
            if (release) {
                delete child;
            }
        }
    }
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

POINT Widget::offset() const {
    POINT p;
    p.x = m_x;
    p.y = m_y;
    if (m_parent && !dynamic_cast<Window*>(m_parent)) {
        auto add = m_parent->offset();
        p.x += add.x;
        p.y += add.y;
    }
    return p;
}

void Widget::updatePosition() {
    this->move(m_x, m_y);
}

void Widget::move(int x, int y) {
    m_x = x;
    m_y = y;
    auto [tx, ty] = this->offset();
    SetWindowPos(m_hwnd, nullptr, tx, ty, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    for (auto& child : m_children) {
        child->updatePosition();
    }
}

void Widget::resize(int w, int h) {
    m_autoresize = false;
    m_width = w;
    m_height = h;
    SetWindowPos(m_hwnd, nullptr, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
}

void Widget::autoresize() {
    m_autoresize = true;
}

void Widget::show() {
    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
}

void Widget::hide() {
    ShowWindow(m_hwnd, SW_HIDE);
    UpdateWindow(m_hwnd);
}

void Widget::paint(DRAWITEMSTRUCT*s) {}

Widget* Widget::getParent() const {
    return m_parent;
}

std::vector<Widget*> Widget::getChildren() const {
    return m_children;
}

HWND Widget::getHWND() const {
    return m_hwnd;
}

Widget::~Widget() {
    if (m_parent) {
        m_parent->remove(this, false);
    }
    for (auto& child : m_children) {
        delete child;
    }
    g_hwnds.erase(m_hwnd);
    if (m_hwnd) DestroyWindow(m_hwnd);
}

void Widget::init(HWND hwnd) {
    g_hwnds[hwnd] = this;
}

Widget* Widget::fromHWND(HWND hwnd) {
    if (!g_hwnds.count(hwnd)) return nullptr;
    return g_hwnds.at(hwnd);
}

void ColorWidget::setColor(COLORREF color) {
    m_color = color;
}

COLORREF ColorWidget::getColor() const {
    return m_color;
}

void TextWidget::setText(std::string const& text) {
    this->m_text = text;
    SetWindowTextA(m_hwnd, text.c_str());
}

std::string TextWidget::getText() const {
    return m_text;
}

void TextWidget::setFont(std::string const& font, int size) {
    m_font = font;
    m_fontsize = size;
    SendMessage(m_hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(Manager::get()->loadFont(font, size)), true);
}

void TextWidget::setFont(std::string const& font) {
    this->setFont(font, m_fontsize);
}

void TextWidget::setFontSize(int size) {
    this->setFont(m_font, size);
}
