#pragma once

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include "../Manager.hpp"

class Widget {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    bool m_autoresize = false;
    Widget* m_parent = nullptr;
    std::vector<Widget*> m_children;
    HWND m_hwnd = nullptr;

    virtual void updateParent();

    void init(HWND);

public:
    virtual ~Widget();

    virtual void paint(DRAWITEMSTRUCT*);

    Widget* getParent() const;
    std::vector<Widget*> getChildren() const;
    HWND getHWND() const;

    virtual void add(Widget* child);
    virtual void remove(Widget* child, bool release = true);
    virtual void resize(int width, int height);
    virtual void autoresize();
    virtual void move(int x, int y);
    virtual void show();
    virtual void hide();

    static Widget* fromHWND(HWND hwnd);
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
