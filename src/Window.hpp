#pragma once

#include <Windows.h>
#include <string>

class Window {
protected:
    std::string m_title;

    bool init(std::string const&, int, int);

public:
    Window(std::string const& title, int width = 600, int height = 400);
    Window() = delete;

    LRESULT proc(HWND, UINT, WPARAM, LPARAM);

    static void run();
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
