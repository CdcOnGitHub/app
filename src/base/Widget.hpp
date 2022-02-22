#pragma once

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include "../Manager.hpp"
#include <utils.hpp>
#include <Style.hpp>
#include <string>
#include <dwmapi.h>
#include <functional>

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
    std::unordered_map<COLORREF, HBRUSH> m_brushes;
    std::unordered_map<std::string, HPEN> m_pens;
    const char* m_type = "Widget";
    static Widget* s_hoveredWidget;
    static Widget* s_capturingWidget;

    void updatePosition();
    void setWindow(Window*);
    Widget* propagateMouseMoveEvent(Point& p, bool down);
    void propagateMouseEvent(Point& p, bool down);
    bool propagateTabEvent(int& index, int target);
    bool propagateCaptureMouse(Point& p);
    void captureMouse();
    void releaseMouse();

    friend class Window;

public:
    virtual ~Widget();

    virtual void paint(HDC hdc, PAINTSTRUCT* ps);
    virtual void updateSize(HDC hdc, SIZE available);

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
    virtual const char* type() const;

    int x() const;
    int y() const;
    int width() const;
    int height() const;
    bool visible() const;

    virtual HCURSOR cursor() const;

    Point offset() const;
    Rect rect() const;
};

class ColorWidget : public Widget {
protected:
    Color m_color;

public:
    virtual void color(Color color);
    Color color() const;
};

class TextWidget : public ColorWidget {
protected:
    std::string m_text;
    std::string m_font;
    int m_fontsize = 20_px;
    bool m_wordwrap = true;

public:
    virtual void text(std::string const& text);
    std::string getText() const;

    void font(std::string const& font);
    virtual void font(std::string const& font, int size);
    virtual void fontSize(int size = 20);
    void wrap(bool);
};
