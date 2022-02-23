#include "Layout.hpp"

Pad::Pad(bool exp) {
    m_typeName = "Pad";
    m_expand = exp;
    this->show();
}

Pad::Pad(int size) {
    m_typeName = "Pad";
    this->resize(size);
    this->show();
}

Pad::Pad() {
    m_typeName = "Pad";
    this->show();
}

void Pad::resize(int size) {
    this->Widget::resize(size, size);
}

void Pad::expand(bool e) {
    m_expand = e;
}

bool Pad::doesExpand() const {
    return m_expand;
}

PadWidget::PadWidget(int size, Widget* widget) {
    m_widget = widget;
    m_widget->autoResize();
    m_widget->show();
    this->add(widget);
    this->pad(size);
    this->show();
}

PadWidget::PadWidget(int size) : PadWidget(size, new HorizontalLayout()) {}

PadWidget::PadWidget() : PadWidget(0, new HorizontalLayout()) {}

Widget* PadWidget::widget() const {
    return m_widget;
}

void PadWidget::add(Widget* child) {
    if (child == m_widget) {
        Widget::add(child);
    } else {
        m_widget->add(child);
    }
}

void PadWidget::remove(Widget* child, bool release) {
    m_widget->remove(child, release);
}

void PadWidget::clear() {
    m_widget->clear();
}

void PadWidget::pad(int size) {
    m_pad = size;
    m_widget->move(m_pad, m_pad);
    this->update();
}

int PadWidget::pad() const {
    return m_pad;
}

void PadWidget::updateSize(HDC hdc, SIZE available) {
    available.cx -= m_pad * 2;
    available.cy -= m_pad * 2;
    Widget::updateSize(hdc, available);
    auto size = m_widget->size();
    this->resize(size.Width + m_pad * 2, size.Height + m_pad * 2);
}

void Layout::updateSize(HDC hdc, SIZE available) {
    Widget::updateSize(hdc, available);
}

void Layout::pad(int p) {
    m_pad = p;
    this->update();
}

HorizontalLayout::HorizontalLayout() {
    m_typeName = "HorizontalLayout";
    this->autoResize();
    this->show();
}

void HorizontalLayout::add(Widget* child, HorizontalLayout::Align alignment) {
    m_alignments[child] = alignment;
    this->add(child);
}

void HorizontalLayout::add(Widget* child) {
    Widget::add(child);
}

void HorizontalLayout::remove(Widget* child, bool release) {
    m_alignments.erase(child);
    Widget::remove(child, release);
}

void HorizontalLayout::fill(bool on) {
    m_fill = on;
    this->update();
}

void HorizontalLayout::align(Align defaultAlign) {
    m_defaultAlign = defaultAlign;
    this->update();
}

void HorizontalLayout::updateSize(HDC hdc, SIZE available) {
    int widths = 0;
    int height = 0;
    int pads = 0;
    auto availableWidth = available.cx;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto pad = dynamic_cast<Pad*>(child);
        if (!pad || !pad->doesExpand()) {
            child->updateSize(hdc, available);
            available.cx -= child->width() + m_pad;
            widths += child->width() + m_pad;
            if (!pad && child->height() > height) {
                height = child->height();
            }
        } else {
            pads++;
        }
    }
    widths -= m_pad;
    if (m_autoresize) {
        m_width = availableWidth;
        m_height = m_fill ? available.cy : height;
    }
    int pos = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto ypos = 0;
        auto alignment = m_defaultAlign;
        if (m_alignments.count(child)) {
            alignment = m_alignments[child];
        }
        switch (alignment) {
            case Align::Start: ypos = 0; break;
            case Align::Middle: ypos = (m_height - child->height()) / 2; break;
            case Align::End: ypos = m_height - child->height(); break;
        }
        child->move(pos, ypos);
        auto pad = dynamic_cast<Pad*>(child);
        if (pad && pad->doesExpand()) {
            pad->resize((m_width - widths) / pads);
        }
        pos += child->width() + m_pad;
    }
}

VerticalLayout::VerticalLayout() {
    m_typeName = "VerticalLayout";
    this->autoResize();
    this->show();
}

void VerticalLayout::add(Widget* child, VerticalLayout::Align alignment) {
    m_alignments[child] = alignment;
    this->add(child);
}

void VerticalLayout::add(Widget* child) {
    Widget::add(child);
}

void VerticalLayout::remove(Widget* child, bool release) {
    m_alignments.erase(child);
    Widget::remove(child, release);
}

void VerticalLayout::fill(bool on) {
    m_fill = on;
    this->update();
}

void VerticalLayout::align(Align defaultAlign) {
    m_defaultAlign = defaultAlign;
    this->update();
}

void VerticalLayout::updateSize(HDC hdc, SIZE available) {
    int heights = 0;
    int width = 0;
    int pads = 0;
    auto availableHeight = available.cy;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto pad = dynamic_cast<Pad*>(child);
        if (!pad || !pad->doesExpand()) {
            child->updateSize(hdc, available);
            available.cy -= child->height() + m_pad;
            heights += child->height() + m_pad;
            if (!pad && child->width() > width) {
                width = child->width();
            }
        } else {
            pads++;
        }
    }
    heights -= m_pad;
    if (m_autoresize) {
        m_width = m_fill ? available.cx : width;
        m_height = availableHeight;
    }
    int pos = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto xpos = 0;
        auto alignment = m_defaultAlign;
        if (m_alignments.count(child)) {
            alignment = m_alignments[child];
        }
        switch (alignment) {
            case Align::Start: xpos = 0; break;
            case Align::Middle: xpos = (m_width - child->width()) / 2; break;
            case Align::End: xpos = m_width - child->width(); break;
        }
        child->move(xpos, pos);
        auto pad = dynamic_cast<Pad*>(child);
        if (pad && pad->doesExpand()) {
            pad->resize((m_height - heights) / pads);
        }
        pos += child->height() + m_pad;
    }
}

ResizeGrip::ResizeGrip(SplitLayout* l) {
    m_typeName = "ResizeGrip";
    m_layout = l;
    this->show();
}

void ResizeGrip::direct(bool horizontal) {
    m_horizontal = horizontal;
    this->update();
}

bool ResizeGrip::horizontal() const {
    return m_horizontal;
}

int ResizeGrip::pos() const {
    return m_pos + m_moved;
}

bool ResizeGrip::wantsMouse() const {
    return true;
}

HCURSOR ResizeGrip::cursor() const {
    return Manager::cursor(
        m_horizontal ?
            (m_layout->m_collapsed ? IDC_HAND : IDC_SIZEWE) :
            (m_layout->m_collapsed ? IDC_HAND : IDC_SIZENS)
    );
}

void ResizeGrip::mouseDown(int x, int y) {
    this->captureMouse();
    m_layout->grow();
    m_mousestart = { x, y };
    m_pos = m_x + s_size;
    this->update();
}

void ResizeGrip::mouseUp(int x, int y) {
    this->releaseMouse();
    this->update();
}

void ResizeGrip::mouseDoubleClick(int x, int y) {
    m_layout->collapse();
}

void ResizeGrip::mouseMove(int x, int y) {
    if (m_mousedown) {
        if (m_horizontal) {
            m_moved = x - m_mousestart.x;
            m_layout->m_split = m_pos + m_moved;
        } else {
            m_moved = y - m_mousestart.y;
            m_layout->m_split = m_pos + m_moved;
        }
        this->update();
    } else {
        this->releaseMouse();
    }
}

void ResizeGrip::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (!m_paintLine) return;
    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    auto r = this->rect();
    SolidBrush brush(Style::separator());
    if (m_horizontal) {
        r.X += r.Width / 2;
        r.Width = 1_px;
        g.FillRectangle(&brush, toRectF(r));
    } else {
        r.Y += r.Height / 2;
        r.Height = 1_px;
        g.FillRectangle(&brush, toRectF(r));
    }
}

void ResizeGrip::hideLine() {
    m_paintLine = false;
    this->update();
}

SplitLayout::SplitLayout() {
    m_typeName = "SplitLayout";
    m_separator = new ResizeGrip(this);
    this->add(m_separator);
    this->show();
}

void SplitLayout::setDirection(bool h) {
    m_horizontal = h;
    m_separator->direct(h);
    this->update();
}

void SplitLayout::first(Widget* w) {
    if (m_first) this->remove(m_first);
    m_first = w;
    this->add(w);
}

void SplitLayout::second(Widget* w) {
    if (m_second) this->remove(m_second);
    m_second = w;
    this->add(w);
}

Widget* SplitLayout::first() const {
    return m_first;
}

Widget* SplitLayout::second() const {
    return m_second;
}

void SplitLayout::moveSplit(int split) {
    m_split = split;
    this->update();
}

void SplitLayout::collapseFirst(bool first) {
    m_collapseFirst = first;
}

void SplitLayout::hideSeparatorLine() {
    m_separator->hideLine();
}

void SplitLayout::paint(HDC hdc, PAINTSTRUCT* ps) {
    if (!(m_collapsed && m_collapseFirst))  m_first->paint(hdc, ps);
    if (!(m_collapsed && !m_collapseFirst)) m_second->paint(hdc, ps);
    m_separator->paint(hdc, ps);
}

void SplitLayout::updateSize(HDC hdc, SIZE size) {
    if (!m_first || !m_second) return;
    if (!m_split) {
        m_split = m_horizontal ? size.cx / 2 : size.cy / 2;
    }
    if (m_min && m_split < m_min) m_split = m_min;
    if (m_max && m_split > m_max) m_split = m_max;
    auto asplit = m_collapsed ? (m_collapseFirst ? 0 : m_width) : m_split;
    auto fsize = size;
    auto ssize = size;
    POINT spos;
    POINT seppos;
    SIZE sepsize;
    if (m_horizontal) {
        fsize.cx = asplit;
        ssize.cx = size.cx - asplit;
        spos.x = asplit;
        spos.y = 0;
        seppos.x = asplit - ResizeGrip::s_size;
        seppos.y = 0;
        sepsize.cx = ResizeGrip::s_size * 2;
        sepsize.cy = size.cy;
    } else {
        fsize.cy = asplit;
        ssize.cy = size.cy - asplit;
        spos.x = 0;
        spos.y = asplit;
        seppos.x = 0;
        seppos.y =  asplit - ResizeGrip::s_size;
        sepsize.cx = size.cx;
        sepsize.cy = ResizeGrip::s_size * 2;
    }
    m_first->updateSize(hdc, fsize);
    m_second->updateSize(hdc, ssize);
    m_second->move(spos.x, spos.y);
    m_separator->move(seppos.x, seppos.y);
    m_separator->resize(sepsize.cx, sepsize.cy);
}

void SplitLayout::min(int m) {
    m_min = m;
    if (m_min && m_split < m_min) m_split = m_min;
}

void SplitLayout::max(int m) {
    m_max = m;
    if (m_max && m_split > m_max) m_split = m_max;
}

void SplitLayout::collapse() {
    m_collapsed = true;
    this->update();
}

void SplitLayout::grow() {
    m_collapsed = false;
    this->update();
}
