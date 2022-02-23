#include "Widget.hpp"
#include <Window.hpp>

Widget* Widget::s_hoveredWidget = nullptr;
Widget* Widget::s_capturingWidget = nullptr;
Widget* Widget::s_keyboardWidget = nullptr;

void Widget::userData(void* data) {
    m_userData = data;
}

void* Widget::userData() const {
    return m_userData;
}

const char* Widget::type() const {
    return m_typeName;
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

void Widget::captureKeyboard() {
    if (s_keyboardWidget) {
        s_keyboardWidget->releaseKeyboard();
    }
    m_keyboardFocused = true;
    s_keyboardWidget = this;
    this->update();
}

void Widget::releaseKeyboard() {
    if (s_keyboardWidget == this) {
        m_keyboardFocused = false;
        s_keyboardWidget = nullptr;
    }
    this->update();
}

void Widget::add(Widget* child) {
    if (!child->m_parent) {
        child->m_parent = this;
        child->setWindow(m_window);
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

void Widget::clear() {
    for (auto& widget : m_children) {
        delete widget;
    }
    m_children.clear();
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

Size Widget::size() const {
    return { m_width, m_height };
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

void Widget::autoResize() {
    m_autoresize = true;
}

void Widget::show(bool v) {
    m_visible = v;
    this->update();
}

void Widget::hide() {
    this->show(false);
}

void Widget::enter() {
    this->update();
}
void Widget::leave() {
    this->update();
}
void Widget::click() {}
void Widget::mouseDoubleClick(int x, int y) {}
void Widget::mouseMove(int x, int y) {}
void Widget::mouseDown(int x, int y) {}
void Widget::mouseUp(int x, int y) {
    this->click();
}

void Widget::keyDown(size_t key, size_t scanCode) {}
void Widget::keyUp(size_t key, size_t scanCode) {}

void Widget::tabEnter() {
    this->m_tabbed = true;
    this->update();
}

void Widget::tabLeave() {
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
                child->tabEnter();
                return true;
            }
            child->tabLeave();
            index++;
        }
        if (child->propagateTabEvent(index, target))
            return true;
    }
    return false;
}

Widget* Widget::propagateMouseEvent(Point& p, bool down, int clickCount) {
    Widget* ret = nullptr;
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        auto child = *it;
        if (child->wantsMouse()) {
            auto r = child->rect();
            if (r.Contains(p)) {
                ret = child;
                switch (clickCount) {
                    case 0: {
                        if (!child->m_hovered) {
                            child->m_hovered = true;
                            child->m_mousedown = down;
                            child->enter();
                        }
                        child->mouseMove(p.X, p.Y);
                    } break;

                    case 1: {
                        if (down) {
                            child->m_mousedown = true;
                            child->mouseDown(p.X, p.Y);
                            child->update();
                        } else {
                            child->m_mousedown = false;
                            child->mouseUp(p.X, p.Y);
                            child->update();
                        }
                    } break;

                    default: {
                        child->m_mousedown = true;
                        child->mouseDown(p.X, p.Y);
                        child->mouseDoubleClick(p.X, p.Y);
                        child->update();
                    } break;
                }
            } else if (clickCount == 0) {
                if (child->m_hovered) {
                    child->m_hovered = false;
                    child->m_mousedown = false;
                    child->leave();
                }
            }
        }
    }
    for (auto& it = m_children.rbegin(); it != m_children.rend(); it++) {
        auto r = (*it)->propagateMouseEvent(p, down, clickCount);
        if (r) ret = r;
    }
    return ret;
}

void Widget::update() {
    if (m_window) {
        m_window->updateWindow();
    }
}

void Widget::updateSize(HDC hdc, SIZE available) {
    for (auto& child : m_children) {
        if (child->m_visible) {
            auto av = available;
            av.cx -= child->m_x;
            av.cy -= child->m_y;
            child->updateSize(hdc, av);
        }
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

void TextWidget::text(std::wstring const& text) {
    m_text = text;
    this->update();
}

void TextWidget::text(std::string const& text) {
    this->text(toWString(text));
}

std::wstring TextWidget::text() const {
    return m_text;
}

void TextWidget::font(std::wstring const& font, int size) {
    m_font = font;
    m_fontSize = size;
    this->update();
}

void TextWidget::font(std::string const& font, int size) {
    this->font(toWString(font), size);
}

void TextWidget::font(std::wstring const& font) {
    this->font(font, m_fontSize);
}

void TextWidget::font(std::string const& font) {
    this->font(font, m_fontSize);
}

void TextWidget::fontSize(int size) {
    this->font(m_font, size);
}

void TextWidget::wrap(bool on) {
    m_wordWrap = on;
    this->update();
}

void TextWidget::style(int style) {
    m_style = style;
    this->update();
}

RectF TextWidget::measureText(
    HDC hdc,
    std::wstring const& text,
    std::wstring const& fontFamily,
    int fontSize,
    int style,
    SIZE const& available,
    StringFormat const& format
) {
    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    RectF r;
    Font font(hdc, Manager::get()->loadFont(fontFamily, fontSize, style));
    if (m_wordWrap) {
        g.MeasureString(
            text.c_str(), -1,
            &font, { 
                0, 0,
                static_cast<REAL>(available.cx),
                static_cast<REAL>(available.cy)
            }, &format, &r
        );
    } else {
        g.MeasureString(
            text.c_str(), -1,
            &font, { 
                0, 0, 0, 0
            }, &format, &r
        );
    }
    if (r.Width > available.cx) r.Width = static_cast<REAL>(available.cx);
    if (r.Height > available.cy) r.Height = static_cast<REAL>(available.cy);
    return r;
}

RectF TextWidget::measureText(
    HDC hdc,
    std::wstring const& text,
    SIZE const& available,
    StringFormat const& format
) {
    return this->measureText(hdc, text, m_font, m_fontSize, m_style, available, format);
}

RectF TextWidget::measureText(HDC hdc, SIZE const& available, StringFormat const& format) {
    return this->measureText(hdc, m_text, m_font, m_fontSize, m_style, available, format);
}

void TextWidget::paintText(
    HDC hdc,
    std::wstring const& text,
    std::wstring const& fontFamily,
    int fontSize,
    int style,
    Color const& color,
    Rect const& drawRect,
    StringFormat const& format
) {
    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    Font font(hdc, Manager::get()->loadFont(fontFamily, fontSize, style));
    SolidBrush brush(color);
    if (m_wordWrap) {
        g.DrawString(
            text.c_str(), -1,
            &font, toRectF(drawRect), &format, &brush
        );
    } else {
        Region reg(drawRect);
        g.SetClip(&reg);
        g.DrawString(
            text.c_str(), -1,
            &font, toPointF(drawRect), &format, &brush
        );
    }
}

void TextWidget::paintText(
    HDC hdc,
    std::wstring const& text,
    int style,
    Color const& color,
    Rect const& drawRect
) {
    return this->paintText(hdc, text, m_font, m_fontSize, style, color, drawRect, StringFormat());
}

void TextWidget::paintText(
    HDC hdc,
    std::wstring const& text,
    Rect const& drawRect,
    StringFormat const& format
) {
    return this->paintText(hdc, text, m_font, m_fontSize, m_style, m_color, drawRect, format);
}

void TextWidget::paintText(HDC hdc, Rect const& drawRect, StringFormat const& format) {
    return this->paintText(hdc, m_text, m_font, m_fontSize, m_style, m_color, drawRect, format);
}

void TextWidget::paint(HDC hdc, PAINTSTRUCT* ps) {
    this->paintText(hdc, this->rect());
    Widget::paint(hdc, ps);
}
