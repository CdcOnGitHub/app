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
