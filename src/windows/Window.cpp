#include "Window.hpp"
#include "../Manager.hpp"
#include <stdexcept>
#include <dwmapi.h>
#include <Button.hpp>
#include <windowsx.h>

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

    m_classID = Manager::get()->acquireWindowClassID();
    auto className = "GeodeAppWindow" + std::to_string(m_classID);

    wcex.cbSize         = sizeof(WNDCLASSEXA);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = Window::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = Manager::get()->getInst();
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = this->brush(Style::BG());
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = className.c_str();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExA(&wcex)) {
        throw std::runtime_error("Unable to register Window Class");
    }

    HWND hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW,
        className.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_EX_LAYERED,
        CW_USEDEFAULT, CW_USEDEFAULT,
        MulDiv(width, Manager::get()->getDPI(), 96),
        MulDiv(height, Manager::get()->getDPI(), 96),
        nullptr,
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
    m_width = width;
    m_height = height;

    g_windows[hwnd] = this;

    this->show();
    this->center();
}

Window::~Window() {
    g_windows.erase(m_hwnd);
    auto className = "GeodeAppWindow" + std::to_string(m_classID);
    UnregisterClassA(className.c_str(), Manager::get()->getInst());
    Manager::get()->relinquishWindowClassID(m_classID);
}

void Window::show() {
    Widget::show();
    ShowWindow(m_hwnd, SW_SHOW);
}

void Window::hide() {
    Widget::hide();
    ShowWindow(m_hwnd, SW_HIDE);
}

void Window::move(int x, int y) {
    Widget::move(x, y);
    SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

HWND Window::getHWND() const {
    return m_hwnd;
}

void Window::updateWindow(RECT rc) {
    InvalidateRect(m_hwnd, &rc, true);
}

void Window::updateWindow() {
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    this->updateWindow(rc);
}

void Window::updateAll() {
    for (auto [_, wnd] : g_windows) {
        wnd->updateWindow();
    }
}

void Window::add(Widget* child) {
    Widget::add(child);
    child->setWindow(this);
    this->updateWindow(child->rect());
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

void Window::paint(HDC hdc, PAINTSTRUCT* ps) {
    FillRect(hdc, &ps->rcPaint, this->brush(Style::BG()));
    Widget::paint(hdc, ps);
}

LRESULT Window::proc(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(m_hwnd, msg, wp, lp);
            if (this->isFullscreen()) return hit;
            
            POINT p;
            p.x = GET_X_LPARAM(lp);
            p.y = GET_Y_LPARAM(lp);
            MapWindowPoints(nullptr, m_hwnd, &p, 1);
            if (this->propagateCaptureMouse(p)) return hit;
            Widget::s_hoveredWidget = this->propagateMouseMoveEvent(p, m_mousedown);
            
            if (hit == HTCLIENT) hit = HTCAPTION;
            
            return hit;
        } break;
    
        case WM_PAINT: {
            PAINTSTRUCT ps;
            auto hdc = BeginPaint(m_hwnd, &ps);
            this->updateSize(hdc, { m_width, m_height });
            this->paint(hdc, &ps);
            EndPaint(m_hwnd, &ps);
            return 0;
        } break;

        case WM_LBUTTONDOWN: {
            POINT p;
            p.x = GET_X_LPARAM(lp);
            p.y = GET_Y_LPARAM(lp);
            m_mousedown = true;
            this->propagateMouseEvent(p, true);
        } break;

        case WM_LBUTTONUP: {
            POINT p;
            p.x = GET_X_LPARAM(lp);
            p.y = GET_Y_LPARAM(lp);
            m_mousedown = false;
            this->propagateMouseEvent(p, false);
        } break;

        case WM_MOUSEMOVE: {
            POINT p;
            p.x = GET_X_LPARAM(lp);
            p.y = GET_Y_LPARAM(lp);
            m_mousedown = wp & MK_LBUTTON;
            Widget::s_hoveredWidget = this->propagateMouseMoveEvent(p, wp & MK_LBUTTON);
        } break;

        case WM_SETCURSOR: {
            if (Widget::s_hoveredWidget) {
                auto cursor = Widget::s_hoveredWidget->cursor();
                if (cursor) {
                    SetCursor(cursor);
                    return 0;
                }
            }
        } break;

        case WM_KEYDOWN: {
            if (wp == VK_TAB) {
                // todo: fix this mess
                int index = 0;
                if (!this->propagateTabEvent(index, m_tabIndex)) {
                    m_tabIndex = 0;
                }
                if (GetKeyState(VK_SHIFT) & 0x8000) {
                    if (m_tabIndex > 0) {
                        m_tabIndex--;
                    }
                } else {
                    m_tabIndex++;
                }
                return 0;
            }
        } break;

        case WM_SIZE: {
            m_fullscreen = wp == SIZE_MAXIMIZED;
            m_width = LOWORD(lp);
            m_height = HIWORD(lp);
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
