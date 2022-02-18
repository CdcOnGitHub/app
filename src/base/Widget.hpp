#pragma once

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include "../Manager.hpp"
#include <utils.hpp>

class Window;

class Widget {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    bool m_autoresize = false;
    bool m_visible = false;
    bool m_hovered = false;
    bool m_mousedown = false;
    bool m_tabbed = false;
    Widget* m_parent = nullptr;
    Window* m_window = nullptr;
    std::vector<Widget*> m_children;
    static HBRUSH s_tabBrush;
    static Widget* s_hoveredWidget;

    void updatePosition();
    void setWindow(Window*);
    Widget* propagateMouseMoveEvent(POINT& p, bool down);
    void propagateMouseEvent(POINT& p, bool down);
    bool propagateTabEvent(int& index, int target);
    bool propagateCaptureMouse(POINT& p);

    friend class Window;

public:
    virtual ~Widget();

    virtual void paint(HDC hdc, PAINTSTRUCT* ps);

    Widget* getParent() const;
    std::vector<Widget*> getChildren() const;

    virtual void add(Widget* child);
    virtual void remove(Widget* child, bool release = true);
    virtual void resize(int width, int height);
    virtual void autoresize();
    virtual void move(int x, int y);
    virtual void show();
    virtual void hide();
    virtual void update();
    virtual void enter();
    virtual void leave();
    void tabenter();
    void tableave();
    virtual void click();
    virtual void mousedown(int x, int y);
    virtual void mouseup(int x, int y);
    virtual void mousemove(int x, int y);
    virtual bool wantsMouse() const;

    virtual HCURSOR cursor() const;

    POINT offset() const;
    RECT rect() const;
};

class ColorWidget : public Widget {
protected:
    COLORREF m_color;

public:
    virtual void setColor(COLORREF color);
    COLORREF getColor() const;
};

class TextWidget : public ColorWidget {
protected:
    std::string m_text;
    std::string m_font;
    int m_fontsize = 20;

public:
    virtual void setText(std::string const& text);
    std::string getText() const;

    void setFont(std::string const& font);
    virtual void setFont(std::string const& font, int size);
    virtual void setFontSize(int size = 20);
};
