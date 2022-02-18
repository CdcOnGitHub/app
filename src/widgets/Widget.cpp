#include "Widget.hpp"
#include <Window.hpp>

void Widget::add(Widget* child) {
    if (!child->m_parent) {
        child->m_parent = this;
        child->m_window = m_window;
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

void Widget::setWindow(Window* window) {
    m_window = window;
    for (auto& child : m_children) {
        child->setWindow(window);
    }
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

RECT Widget::rect() const {
    auto p = this->offset();
    RECT r;
    r.left = p.x;
    r.top = p.y;
    r.right = p.x + m_width;
    r.bottom = p.y + m_height;
    return r;
}

void Widget::updatePosition() {
    this->move(m_x, m_y);
}

void Widget::move(int x, int y) {
    m_x = x;
    m_y = y;
    for (auto& child : m_children) {
        child->updatePosition();
    }
}

void Widget::resize(int w, int h) {
    m_autoresize = false;
    m_width = w;
    m_height = h;
}

void Widget::autoresize() {
    m_autoresize = true;
}

void Widget::show() {
    m_visible = true;
}

void Widget::hide() {
    m_visible = false;
}

void Widget::enter() {}
void Widget::leave() {}
void Widget::click() {}
void Widget::mousemove(int x, int y) {}
void Widget::mousedown(int x, int y) {}
void Widget::mouseup(int x, int y) {
    this->click();
}

bool Widget::wantsMouse() const {
    return false;
}

bool Widget::propagateCaptureMouse(POINT& p) {
    auto r = this->rect();
    if (PtInRect(&r, p) && this->wantsMouse()) return true;
    for (auto& child : m_children) {
        if (child->propagateCaptureMouse(p))
            return true;
    }
    return false;
}

void Widget::propagateMouseEvent(POINT& p, bool down) {
    for (auto& child : m_children) {
        auto r = child->rect();
        if (PtInRect(&r, p)) {
            if (down) {
                child->m_mousedown = true;
                child->mousedown(p.x, p.y);
                child->update();
            } else {
                child->m_mousedown = false;
                child->mouseup(p.x, p.y);
                child->update();
            }
        }
        child->propagateMouseEvent(p, down);
    }
}

void Widget::propagateMouseMoveEvent(POINT& p, bool down) {
    for (auto& child : m_children) {
        auto r = child->rect();
        if (PtInRect(&r, p)) {
            if (!child->m_hovered) {
                child->m_hovered = true;
                child->m_mousedown = down;
                child->enter();
            }
            child->mousemove(p.x, p.y);
        } else {
            if (child->m_hovered) {
                child->m_hovered = false;
                child->m_mousedown = false;
                child->leave();
            }
        }
        child->propagateMouseMoveEvent(p, down);
    }
}

void Widget::update() {
    if (m_window) m_window->updateWindow(this->rect());
}

void Widget::paint(HDC hdc, PAINTSTRUCT* ps) {
    for (auto& child : m_children) {
        if (child->m_visible) child->paint(hdc, ps);
    }
}

Widget* Widget::getParent() const {
    return m_parent;
}

std::vector<Widget*> Widget::getChildren() const {
    return m_children;
}

Widget::~Widget() {
    if (m_parent) {
        m_parent->remove(this, false);
    }
    for (auto& child : m_children) {
        delete child;
    }
}

void ColorWidget::setColor(COLORREF color) {
    m_color = color;
}

COLORREF ColorWidget::getColor() const {
    return m_color;
}

void TextWidget::setText(std::string const& text) {
    this->m_text = text;
}

std::string TextWidget::getText() const {
    return m_text;
}

void TextWidget::setFont(std::string const& font, int size) {
    m_font = font;
    m_fontsize = size;
}

void TextWidget::setFont(std::string const& font) {
    this->setFont(font, m_fontsize);
}

void TextWidget::setFontSize(int size) {
    this->setFont(m_font, size);
}
