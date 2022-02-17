#include "Window.hpp"
#include "../Manager.hpp"
#include <stdexcept>
#include <dwmapi.h>
#include <Button.hpp>

static std::unordered_map<HWND, Window*> g_windows;

enum ACCENTFLAGS {
    AF_DRAWNOBORDERS = 0x0,
    AF_DRAWLEFTBORDER = 0x20,
    AF_DRAWTOPBORDER = 0x40,
    AF_DRAWRIGHTBORDER = 0x80,
    AF_DRAWBOTTOMBORDER = 0x100,
    AF_DRAWALLBORDERS = AF_DRAWLEFTBORDER | AF_DRAWTOPBORDER | AF_DRAWRIGHTBORDER | AF_DRAWBOTTOMBORDER
};

enum ACCENTSTATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_INVALID_STATE = 4
};

static constexpr const auto WCA_ACCENT_POLICY = 19;

struct ACCENTPOLICY {
    ACCENTSTATE eAccentState;
    ACCENTFLAGS eAccentFlags;
    int nGradientColor;
    int nAnimationId;
};
struct WINCOMPATTRDATA {
    int nAttribute;
    PVOID pvData;
    ULONG ulSizeOfData;
};

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
    wcex.hbrBackground  = CreateSolidBrush(RGB(0x26, 0x24, 0x2d));
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
        WS_OVERLAPPEDWINDOW | WS_EX_LAYERED,
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

    BOOL USE_DARK_MODE = true;
    DwmSetWindowAttribute(hwnd, 20, &USE_DARK_MODE, sizeof(USE_DARK_MODE));

    // const HINSTANCE hm = LoadLibraryA("user32.dll");
    // typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
    // const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
    // if (SetWindowCompositionAttribute) {
    //     ACCENTPOLICY policy = { ACCENT_ENABLE_BLURBEHIND, AF_DRAWNOBORDERS, 0, 0 };
    //     WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) };
    //     SetWindowCompositionAttribute(hwnd, &data);
    // }
    // FreeLibrary(hm);

    m_hwnd = hwnd;

    g_windows[hwnd] = this;

    this->show();
    this->center();
}

Window::~Window() {
    g_windows.erase(m_hwnd);
}

void Window::center() {
    RECT rc;
    GetWindowRect(m_hwnd, &rc);
    this->move(
        (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2
    );
}

bool Window::isFullscreen() const {
    return m_fullscreen;
}

LRESULT Window::proc(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(m_hwnd, msg, wp, lp);
            if (this->isFullscreen()) return hit;
            if (hit == HTCLIENT) hit = HTCAPTION;
            return hit;
        } break;

        case WM_DRAWITEM: {
            auto dis = reinterpret_cast<DRAWITEMSTRUCT*>(lp);
            auto widget = Widget::fromHWND(dis->hwndItem);
            if (widget) widget->paint(dis);
        } break;

        case WM_COMMAND: {
            auto btn = Button::fromHMENU(reinterpret_cast<HMENU>(LOWORD(wp)));
            if (btn) btn->click();
        } break;

        case WM_SIZE: {
            m_fullscreen = wp == SIZE_MAXIMIZED;
        } break;
    }
    return DefWindowProc(m_hwnd, msg, wp, lp);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY && hwnd == Manager::get()->getMainWindow()->getHWND()) {
        PostQuitMessage(0);
        return 0;
    }
    if (g_windows.count(hwnd)) {
        return g_windows.at(hwnd)->proc(msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
