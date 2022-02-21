#include "Widget.hpp"
#include <Window.hpp>

Widget* Widget::s_hoveredWidget = nullptr;
Widget* Widget::s_capturingWidget = nullptr;

const char* Widget::type() const {
    return m_type;
}

void Widget::captureMouse() {
    if (!s_capturingWidget) {
        s_capturingWidget = this;
    }
}

void Widget::releaseMouse() {
    if (s_capturingWidget == this) {
        s_capturingWidget = nullptr;
    }
}

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

Point Widget::offset() const {
    Point p;
    p.X = m_x;
    p.Y = m_y;
    if (m_parent && !dynamic_cast<Window*>(m_parent)) {
        auto add = m_parent->offset();
        p.X += add.X;
        p.Y += add.Y;
    }
    return p;
}

Rect Widget::rect() const {
    auto p = this->offset();
    Rect r;
    r.X = p.X;
    r.Y = p.Y;
    r.Width = m_width;
    r.Height = m_height;
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
    this->update();
}

void Widget::hide() {
    m_visible = false;
    this->update();
}

void Widget::enter() {
    this->update();
}
void Widget::leave() {
    this->update();
}
void Widget::click() {}
void Widget::mousemove(int x, int y) {}
void Widget::mousedown(int x, int y) {}
void Widget::mouseup(int x, int y) {
    this->click();
}

void Widget::tabenter() {
    this->m_tabbed = true;
    this->update();
}

void Widget::tableave() {
    this->m_tabbed = false;
    this->update();
}

bool Widget::wantsMouse() const {
    return false;
}

bool Widget::propagateCaptureMouse(Point& p) {
    auto r = this->rect();
    if (r.Contains(p) && this->wantsMouse()) return true;
    for (auto& child : m_children) {
        if (child->propagateCaptureMouse(p))
            return true;
    }
    return false;
}

bool Widget::propagateTabEvent(int& index, int target) {
    for (auto& child : m_children) {
        if (child->m_visible && child->wantsMouse()) {
            if (index == target) {
                child->tabenter();
                return true;
            }
            child->tableave();
            index++;
        }
        if (child->propagateTabEvent(index, target))
            return true;
    }
    return false;
}

void Widget::propagateMouseEvent(Point& p, bool down) {
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        auto child = *it;
        if (child->wantsMouse()) {
            auto r = child->rect();
            if (r.Contains(p)) {
                if (down) {
                    child->m_mousedown = true;
                    child->mousedown(p.X, p.Y);
                    child->update();
                } else {
                    child->m_mousedown = false;
                    child->mouseup(p.X, p.Y);
                    child->update();
                }
            }
        }
    }
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        (*it)->propagateMouseEvent(p, down);
    }
}

Widget* Widget::propagateMouseMoveEvent(Point& p, bool down) {
    Widget* ret = nullptr;
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        auto child = *it;
        if (child->wantsMouse()) {
            auto r = child->rect();
            if (r.Contains(p)) {
                if (!child->m_hovered) {
                    child->m_hovered = true;
                    child->m_mousedown = down;
                    child->enter();
                }
                ret = child;
                child->mousemove(p.X, p.Y);
            } else {
                if (child->m_hovered) {
                    child->m_hovered = false;
                    child->m_mousedown = false;
                    child->leave();
                }
            }
        }
    }
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        auto r = (*it)->propagateMouseMoveEvent(p, down);
        if (r) ret = r;
    }
    return ret;
}

void Widget::update() {
    if (m_window) m_window->updateWindow();
}

void Widget::updateSize(HDC hdc, SIZE size) {
    for (auto& child : m_children) {
        if (child->m_visible) child->updateSize(hdc, size);
    }
}

void Widget::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (m_tabbed) {
        Pen pen(Style::tab());
        Graphics(hdc).DrawRectangle(&pen, toRectF(this->rect()));
    }
    for (auto& child : m_children) {
        if (child->m_visible) child->paint(hdc, ps);
    }
}

HCURSOR Widget::cursor() const {
    return nullptr;
}

Widget* Widget::getParent() const {
    return m_parent;
}

std::vector<Widget*> Widget::getChildren() const {
    return m_children;
}

Widget::~Widget() {
    for (auto [_, b] : m_brushes) {
        DeleteObject(b);
    }
    for (auto [_, b] : m_pens) {
        DeleteObject(b);
    }
    for (auto& child : m_children) {
        delete child;
    }
}

int Widget::x() const { return m_x; }
int Widget::y() const { return m_y; }
int Widget::width() const { return m_width; }
int Widget::height() const { return m_height; }
bool Widget::visible() const { return m_visible; }

void ColorWidget::color(Color color) {
    m_color = color;
}

Color ColorWidget::color() const {
    return m_color;
}

void TextWidget::text(std::string const& text) {
    this->m_text = text;
}

std::string TextWidget::getText() const {
    return m_text;
}

void TextWidget::font(std::string const& font, int size) {
    m_font = font;
    m_fontsize = size;
}

void TextWidget::font(std::string const& font) {
    this->font(font, m_fontsize);
}

void TextWidget::fontSize(int size) {
    this->font(m_font, size);
}
