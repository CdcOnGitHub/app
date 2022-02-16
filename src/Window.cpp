#include "Window.hpp"
#include "Manager.hpp"

bool Window::init(std::string const& title, int width, int height) {
    HWND hwnd = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW,
        g_windowClassName,
        L"GeodeApp",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        Manager::get()->inst(),
        nullptr
    );

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return true;
}

Window::Window(std::string const& title, int width, int height) {
    this->init(title, width, height);
}

LRESULT Window::proc(HWND, UINT, WPARAM, LPARAM) {
    return 0;
}

void Window::run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
    }
    return 0;
}
