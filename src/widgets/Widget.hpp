#pragma once

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include "../Manager.hpp"

class Widget {
protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    Widget* m_parent = nullptr;
    std::vector<Widget*> m_children;
    HWND m_hwnd = nullptr;

public:
    virtual ~Widget();

    virtual void paint();

    Widget* getParent() const;
    std::vector<Widget*> getChildren() const;
    HWND getHWND() const;

    void add(Widget* child);
    void remove(Widget* child, bool release = true);
    void resize(int width, int height);
    void move(int x, int y);
    void show();
    void hide();

    virtual LRESULT proc(UINT msg, WPARAM wparam, LPARAM lparam);
};
