#include "Window.hpp"
#include "../Manager.hpp"
#include <stdexcept>

static std::unordered_map<HWND, Window*> g_windows;

Window::Window(std::string const& title, int width, int height) {
    WNDCLASSEXA wcex;

    wcex.cbSize         = sizeof(WNDCLASSEXA);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = Window::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = Manager::get()->getInst();
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = g_windowClassName;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExA(&wcex)) {
        throw std::runtime_error("Unable to register Window Class");
    }

    auto parent = Manager::get()->getMainWindow();
    HWND hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW,
        g_windowClassName,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        parent ? parent->getHWND() : nullptr,
        nullptr,
        Manager::get()->getInst(),
        nullptr
    );

    if (!hwnd) {
        throw std::runtime_error("Unable to create Window");
    }

    m_hwnd = hwnd;

    this->show();
    this->center();
}

void Window::center() {
    RECT rc;
    GetWindowRect(m_hwnd, &rc);
    this->move(
        (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2
    );
}

LRESULT CALLBACK Window::recurseWndProc(Widget* widget, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    for (auto w : widget->getChildren()) {
        w->proc(msg, wp, lp);
        recurseWndProc(w, hwnd, msg, wp, lp);
    }
    return 0;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY && hwnd == Manager::get()->getMainWindow()->getHWND()) {
        PostQuitMessage(0);
    }
    for (auto& [h, wnd] : g_windows) {
        if (hwnd == h) {
            wnd->proc(msg, wParam, lParam);
            recurseWndProc(wnd, hwnd, msg, wParam, lParam);
        }
    }
    if (g_windows.count(hwnd)) {
        return g_windows[hwnd]->proc(msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
