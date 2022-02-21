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
};

class Tab : public TextWidget {
public:
    static int s_pad;
    static int s_height;
    static int s_dot;

protected:
    bool m_selected = false;
    Color m_dotColor;

public:
    Tab(std::string const& text);

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
