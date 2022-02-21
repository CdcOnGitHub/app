#include "Layout.hpp"

Pad::Pad(bool exp) {
    m_type = "Pad";
    m_expand = exp;
    this->show();
}

Pad::Pad(int size) {
    m_type = "Pad";
    this->resize(size);
    this->show();
}

Pad::Pad() {
    m_type = "Pad";
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

void Layout::updateSize(HDC hdc, SIZE available) {
    Widget::updateSize(hdc, available);
    this->arrange(available);
}

void Layout::pad(int p) {
    m_pad = p;
    this->update();
}

HorizontalLayout::HorizontalLayout() {
    m_type = "HorizontalLayout";
    this->autoresize();
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

void HorizontalLayout::arrange(SIZE available) {
    int widths = 0;
    int height = 0;
    int pads = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto pad = dynamic_cast<Pad*>(child);
        if (!pad || !pad->doesExpand()) {
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
        m_width = available.cx;
        m_height = height;
    }
    int pos = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto ypos = 0;
        if (m_alignments.count(child)) {
            switch (m_alignments[child]) {
                // case Align::Start: ypos = 0; break;
                case Align::Middle: ypos = (m_height - child->height()) / 2; break;
                case Align::End: ypos = m_height - child->height(); break;
            }
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
    m_type = "VerticalLayout";
    this->autoresize();
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

void VerticalLayout::arrange(SIZE available) {
    int heights = 0;
    int width = 0;
    int pads = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto pad = dynamic_cast<Pad*>(child);
        if (!pad || !pad->doesExpand()) {
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
        m_width = width;
        m_height = available.cy;
    }
    int pos = 0;
    for (auto& child : m_children) {
        if (!child->visible()) continue;
        auto xpos = 0;
        if (m_alignments.count(child)) {
            switch (m_alignments[child]) {
                // case Align::Start: xpos = 0; break;
                case Align::Middle: xpos = (m_width - child->width()) / 2; break;
                case Align::End: xpos = m_width - child->width(); break;
            }
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
    m_type = "ResizeGrip";
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
    return Manager::cursor(m_horizontal ? IDC_SIZEWE : IDC_SIZENS);
}

void ResizeGrip::mousedown(int x, int y) {
    this->captureMouse();
    m_mousestart = { x, y };
    m_pos = m_x + s_size;
    this->update();
}

void ResizeGrip::mouseup(int x, int y) {
    this->releaseMouse();
    this->update();
}

void ResizeGrip::mousemove(int x, int y) {
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
    if (m_horizontal) {
        r.X += r.Width / 2;
        r.Width = 1_px;
        g.FillRectangle(&SolidBrush(Style::separator()), toRectF(r));
    } else {
        r.Y += r.Height / 2;
        r.Height = 1_px;
        g.FillRectangle(&SolidBrush(Style::separator()), toRectF(r));
    }
}

void ResizeGrip::hideline() {
    m_paintLine = false;
    this->update();
}

SplitLayout::SplitLayout() {
    m_type = "SplitLayout";
    m_separator = new ResizeGrip(this);
    this->add(m_separator);
    this->show();
}

void SplitLayout::setDirection(bool h) {
    m_horizontal = h;
    m_separator->direct(h);
    this->update();
}

void SplitLayout::arrange(SIZE available) {}

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
    m_separator->hideline();
}

void SplitLayout::paint(HDC hdc, PAINTSTRUCT* ps) {
    m_first->paint(hdc, ps);
    m_second->paint(hdc, ps);
    m_separator->paint(hdc, ps);
}

void SplitLayout::updateSize(HDC hdc, SIZE size) {
    if (!m_first || !m_second) return;
    if (!m_split) {
        m_split = m_horizontal ? size.cx / 2 : size.cy / 2;
    }
    auto fsize = size;
    auto ssize = size;
    POINT spos;
    POINT seppos;
    SIZE sepsize;
    if (m_horizontal) {
        fsize.cx = m_split;
        ssize.cx = size.cx - m_split;
        spos.x = m_split;
        spos.y = 0;
        seppos.x = m_split - ResizeGrip::s_size;
        seppos.y = 0;
        sepsize.cx = ResizeGrip::s_size * 2;
        sepsize.cy = size.cy;
    } else {
        fsize.cy = m_split;
        ssize.cy = size.cy - m_split;
        spos.x = 0;
        spos.y = m_split;
        seppos.x = 0;
        seppos.y =  m_split - ResizeGrip::s_size;
        sepsize.cx = size.cx;
        sepsize.cy = ResizeGrip::s_size * 2;
    }
    m_first->updateSize(hdc, fsize);
    m_second->updateSize(hdc, ssize);
    m_second->move(spos.x, spos.y);
    m_separator->move(seppos.x, seppos.y);
    m_separator->resize(sepsize.cx, sepsize.cy);
}
