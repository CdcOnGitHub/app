#pragma once

#include "Widget.hpp"

class Pad : public Widget {
protected:
    bool m_expand = false;

public:
    Pad(bool expand = false);
    Pad(int size);
    Pad();

    void resize(int size);
    void expand(bool exp = true);
    bool doesExpand() const;
};

class Layout : public Widget {
protected:
    int m_pad = 0;

public:
    virtual void arrange(SIZE) = 0;

    void pad(int pad);
    void updateSize(HDC, SIZE) override;
};

class HorizontalLayout : public Layout {
public:
    enum Align {
        Start,
        Middle,
        End,
    };

protected:
    std::unordered_map<Widget*, Align> m_alignments;

public:
    HorizontalLayout();

    void add(Widget* child) override;
    void add(Widget* child, Align alignment);
    void remove(Widget* child, bool release = true) override;
    void arrange(SIZE available) override;
};

class VerticalLayout : public Layout {
public:
    enum Align {
        Start,
        Middle,
        End,
    };

protected:
    std::unordered_map<Widget*, Align> m_alignments;

public:
    VerticalLayout();

    void add(Widget* child) override;
    void add(Widget* child, Align alignment);
    void remove(Widget* child, bool release = true) override;
    void arrange(SIZE available) override;
};

class SplitLayout;

class ResizeGrip : public Widget {
public:
    static constexpr const int s_size = 12;

protected:
    bool m_horizontal = true;
    bool m_paintLine = true;
    int m_pos = 0;
    int m_moved = 0;
    POINT m_mousestart;
    SplitLayout* m_layout;

    friend class SplitLayout;

public:
    ResizeGrip(SplitLayout* layout);

    void direct(bool horizontal);
    bool horizontal() const;
    int pos() const;

    bool wantsMouse() const override;
    HCURSOR cursor() const;
    void mousedown(int x, int y) override;
    void mouseup(int x, int y) override;
    void mousemove(int x, int y) override;
    void hideline();

    void paint(HDC, PAINTSTRUCT*) override;
};

class SplitLayout : public Layout {
protected:
    bool m_horizontal = true;
    bool m_collapseFirst = true;
    int m_split = 0;
    Widget* m_first = nullptr;
    Widget* m_second = nullptr;
    ResizeGrip* m_separator = nullptr;

    friend class ResizeGrip;

public:
    SplitLayout();

    void setDirection(bool horizontal);
    void arrange(SIZE available) override;
    void first(Widget* w);
    void second(Widget* w);
    Widget* first() const;
    Widget* second() const;
    void moveSplit(int split);
    void collapseFirst(bool first = true);
    void paint(HDC, PAINTSTRUCT*) override;
    void hideSeparatorLine();

    void updateSize(HDC, SIZE) override;
};
