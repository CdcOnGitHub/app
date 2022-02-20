#include "Layout.hpp"

Pad::Pad(bool exp) {
    m_expand = exp;
    this->show();
}

Pad::Pad(int size) {
    this->resize(size);
    this->show();
}

Pad::Pad() {
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
    this->arrange(available);
    Widget::updateSize(hdc, available);
}

void Layout::pad(int p) {
    m_pad = p;
    this->update();
}

HorizontalLayout::HorizontalLayout() {
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
