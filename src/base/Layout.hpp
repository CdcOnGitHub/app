#pragma once

#include "Widget.hpp"
#undef min
#undef max

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

class PadWidget : public Widget {
protected:
    int m_pad = 0;
    Widget* m_widget = nullptr;

public:
    PadWidget(int size, Widget* widget);
    PadWidget(int size);
    PadWidget();

    void pad(int size);
    int pad() const;

    Widget* widget() const;
    Widget* widget(Widget* other, bool releaseOld = true);

    void updateSize(HDC, SIZE) override;
    void add(Widget* child) override;
    void remove(Widget* child, bool release = true) override;
    void clear() override;
};

class Layout : public Widget {
protected:
    int m_pad = 0;

public:
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
    bool m_fill = false;
    bool m_inverted = false;
    Align m_defaultAlign = Align::Start;
    std::unordered_map<Widget*, Align> m_alignments;

public:
    HorizontalLayout();

    void fill(bool on = true);
    void invert(bool on = true);
    void align(Align defaultAlign);
    void updateSize(HDC, SIZE) override;
    void add(Widget* child) override;
    void add(Widget* child, Align alignment);
    void remove(Widget* child, bool release = true) override;
};

class VerticalLayout : public Layout {
public:
    enum Align {
        Start,
        Middle,
        End,
    };

protected:
    bool m_fill = false;
    bool m_inverted = false;
    Align m_defaultAlign = Align::Start;
    std::unordered_map<Widget*, Align> m_alignments;

public:
    VerticalLayout();

    void fill(bool on = true);
    void invert(bool on = true);
    void align(Align defaultAlign);
    void updateSize(HDC, SIZE) override;
    void add(Widget* child) override;
    void add(Widget* child, Align alignment);
    void remove(Widget* child, bool release = true) override;
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
    void mouseDown(int x, int y) override;
    void mouseUp(int x, int y) override;
    void mouseMove(int x, int y) override;
    void mouseDoubleClick(int x, int y) override;
    void hideLine();

    void paint(HDC, PAINTSTRUCT*) override;
};

class SplitLayout : public Layout {
protected:
    bool m_horizontal = true;
    bool m_collapseFirst = true;
    bool m_collapsed = false;
    int m_split = 0;
    int m_min = 0;
    int m_max = 0;
    Widget* m_first = nullptr;
    Widget* m_second = nullptr;
    ResizeGrip* m_separator = nullptr;

    friend class ResizeGrip;

public:
    SplitLayout();

    void setDirection(bool horizontal);
    void first(Widget* w);
    void second(Widget* w);
    Widget* first() const;
    Widget* second() const;
    void moveSplit(int split);
    void collapseFirst(bool first = true);
    void paint(HDC, PAINTSTRUCT*) override;
    void hideSeparatorLine();
    void min(int m);
    void max(int m);
    void collapse();
    void grow();

    void updateSize(HDC, SIZE) override;
};
