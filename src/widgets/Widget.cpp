#include "Widget.hpp"

void Widget::add(Widget* child) {
    if (!child->m_parent) {
        child->m_parent = this;
        SetParent(child->m_hwnd, this->m_hwnd);
        SetWindowLongPtrA(child->m_hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
        this->m_children.push_back(child);
        UpdateWindow(child->m_hwnd);
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

void Widget::move(int x, int y) {
    SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Widget::resize(int w, int h) {
    SetWindowPos(m_hwnd, nullptr, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
}

void Widget::show() {
    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
}

void Widget::hide() {
    ShowWindow(m_hwnd, SW_HIDE);
    UpdateWindow(m_hwnd);
}

void Widget::paint() {}

LRESULT Widget::proc(UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_PAINT) this->paint();
    return DefWindowProc(m_hwnd, msg, wp, lp);
}

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
    if (m_hwnd) DestroyWindow(m_hwnd);
}

