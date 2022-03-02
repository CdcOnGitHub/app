#pragma once

#include <Widget.hpp>
#include <Layout.hpp>

class Tab;

class Tabs : public Widget {
public:
    using SelectFunc = std::function<void(size_t)>;

protected:
    std::vector<Tab*> m_tabs;
    Layout* m_layout;
    SelectFunc m_func = nullptr;

public:
    Tabs(Layout* l);

    void onSelect(SelectFunc func);
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
        Plus,
    };

    using Callback = std::function<void(Tab*)>;

protected:
    bool m_selected = false;
    bool m_arrow = true;
    bool m_button = false;
    Color m_dotColor;
    Type m_type;
    Tabs* m_control = nullptr;
    size_t m_id;
    Callback m_callback = nullptr;

    friend class Tabs;

public:
    Tab(size_t id, std::string const& text, Type type = Type::Dot);
    Tab(std::string const& text, Type type = Type::Dot);

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;

    void callback(Callback);
    void makeButton(bool b);
    void arrow(bool a);
    static Color dot();

    void click() override;
};

class Separator : public Widget {
protected:
    bool m_pad;
    int m_size;
    int m_drawSize;

public:
    Separator(bool pad = true, int size = 20_px, int drawSize = 2_px);

    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    void pad(bool = true);
    void size(int);
    void drawSize(int);
};

constexpr size_t operator"" _id(const char* str, size_t) {
    return const_hash(str);
}
