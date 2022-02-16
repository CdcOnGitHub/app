#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include "../widgets/Widget.hpp"

class Window : public Widget {
protected:
    std::string m_title;

public:
    Window(std::string const& title, int width = 600, int height = 400);
    Window() = delete;

    void center();
    void setTitle(std::string const&);

    static LRESULT CALLBACK recurseWndProc(Widget*, HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
