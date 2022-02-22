#pragma once

#include <Widget.hpp>
#include <Layout.hpp>

class Tab;

class Tabs : public Widget {
protected:
    std::vector<Tab*> m_tabs;
    Layout* m_layout;

public:
    Tabs(Layout* l);

    void add(Widget* w) override;
    void select(Tab* tab);
};

class Tab : public TextWidget {
public:
    static int s_pad;
    static int s_height;
    static int s_dot;
    static int s_arrow;
    
    enum Type {
        Diamond,
        Dot,
    };

protected:
    bool m_selected = false;
    Color m_dotColor;
    Type m_type;
    Tabs* m_control = nullptr;

    friend class Tabs;

public:
    Tab(std::string const& text, Type type = Type::Dot);

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;

    static Color dot();

    void click() override;
};

class TabSeparator : public Widget {
public:
    TabSeparator();

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};
