#include "Window.hpp"
#include "../Manager.hpp"
#include <stdexcept>
#include <dwmapi.h>
#include <Button.hpp>
#include <windowsx.h>

static std::unordered_map<HWND, Window*> g_windows;

typedef enum _WINDOWCOMPOSITIONATTRIB
{
	WCA_UNDEFINED = 0,
	WCA_NCRENDERING_ENABLED = 1,
	WCA_NCRENDERING_POLICY = 2,
	WCA_TRANSITIONS_FORCEDISABLED = 3,
	WCA_ALLOW_NCPAINT = 4,
	WCA_CAPTION_BUTTON_BOUNDS = 5,
	WCA_NONCLIENT_RTL_LAYOUT = 6,
	WCA_FORCE_ICONIC_REPRESENTATION = 7,
	WCA_EXTENDED_FRAME_BOUNDS = 8,
	WCA_HAS_ICONIC_BITMAP = 9,
	WCA_THEME_ATTRIBUTES = 10,
	WCA_NCRENDERING_EXILED = 11,
	WCA_NCADORNMENTINFO = 12,
	WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
	WCA_VIDEO_OVERLAY_ACTIVE = 14,
	WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
	WCA_DISALLOW_PEEK = 16,
	WCA_CLOAK = 17,
	WCA_CLOAKED = 18,
	WCA_ACCENT_POLICY = 19,
	WCA_FREEZE_REPRESENTATION = 20,
	WCA_EVER_UNCLOAKED = 21,
	WCA_VISUAL_OWNER = 22,
	WCA_HOLOGRAPHIC = 23,
	WCA_EXCLUDED_FROM_DDA = 24,
	WCA_PASSIVEUPDATEMODE = 25,
	WCA_USEDARKMODECOLORS = 26,
	WCA_CORNER_STYLE = 27,
	WCA_PART_COLOR = 28,
	WCA_DISABLE_MOVESIZE_FEEDBACK = 29,
	WCA_LAST = 30
} WINDOWCOMPOSITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA
{
	WINDOWCOMPOSITIONATTRIB Attrib;
	PVOID pvData;
	SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef enum _ACCENT_STATE
{
	ACCENT_DISABLED = 0,
	ACCENT_ENABLE_GRADIENT = 1,
	ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
	ACCENT_ENABLE_BLURBEHIND = 3,
	ACCENT_ENABLE_ACRYLICBLURBEHIND = 4, // RS4 1803
	ACCENT_ENABLE_HOSTBACKDROP = 5, // RS5 1809
 	ACCENT_INVALID_STATE = 6
} ACCENT_STATE;

typedef struct _ACCENT_POLICY
{
	ACCENT_STATE AccentState;
	DWORD AccentFlags;
	DWORD GradientColor;
	DWORD AnimationId;
} ACCENT_POLICY;

typedef BOOL (WINAPI *pfnGetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

typedef BOOL (WINAPI *pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

Window::Window(std::string const& title, bool hasParent, int width, int height) {
    WNDCLASSEXA wcex;

    m_classID = Manager::get()->acquireWindowClassID();
    auto className = "GeodeAppWindow" + std::to_string(m_classID);

    wcex.cbSize         = sizeof(WNDCLASSEXA);
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = Window::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = Manager::get()->getInst();
    wcex.hIcon          = LoadIcon(wcex.hInstance, "applogo");
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = className.c_str();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExA(&wcex)) {
        throw std::runtime_error("Unable to register Window Class");
    }

    auto parent = Manager::get()->getMainWindow();
    if (hasParent) m_window = parent;
    HWND hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW,
        className.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW | (hasParent ? WS_POPUP : WS_EX_LAYERED),
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        (hasParent ? (parent ? parent->getHWND() : nullptr) : nullptr),
        nullptr,
        Manager::get()->getInst(),
        nullptr
    );

    if (!hwnd) {
        throw std::runtime_error("Unable to create Window");
    }

    BOOL USE_DARK_MODE = true;
    DwmSetWindowAttribute(hwnd, 20, &USE_DARK_MODE, sizeof(USE_DARK_MODE));

    m_hwnd = hwnd;
    m_typeName = "Window";
    m_width = width;
    m_height = height;

    g_windows[hwnd] = this;

    this->show();
    this->center();
}

Window::Window(std::string const& title, int width, int height) :
    Window(title, false, width, height) {}

Window::~Window() {
    g_windows.erase(m_hwnd);
    DestroyWindow(m_hwnd);
    auto className = "GeodeAppWindow" + std::to_string(m_classID);
    if (UnregisterClassA(className.c_str(), Manager::get()->getInst())) {
        Manager::get()->relinquishWindowClassID(m_classID);
    }
}

void Window::show(bool v) {
    Widget::show(v);
    ShowWindow(m_hwnd, v ? SW_SHOW : SW_HIDE);
}

void Window::move(int x, int y) {
    Widget::move(x, y);
    SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

HWND Window::getHWND() const {
    return m_hwnd;
}

void Window::updateWindow(RECT rc) {
    InvalidateRect(m_hwnd, &rc, false);
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
    this->updateWindow(toRECT(child->rect()));
}

void Window::center() {
    if (m_window) {
        this->move(
            m_window->x() + (m_window->width() - m_width) / 2,
            m_window->y() + (m_window->height() - m_height) / 2
        );
    } else {
        RECT rc;
        GetWindowRect(m_hwnd, &rc);
        this->move(
            (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2
        );
    }
}

bool Window::isFullscreen() const {
    return m_fullscreen;
}

void Window::paint(HDC hdc, PAINTSTRUCT* ps) {
    Graphics g(hdc);
    g.FillRectangle(
        &SolidBrush(Style::BG()),
        0, 0, m_width, m_height
    );
    Widget::paint(hdc, ps);
}

const int EXTEND_TOP = 40;
const int EXTEND_SIDE = 8;

LRESULT Window::proc(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND: {
            // wmId = LOWORD(wp);
            // wmEvent = HIWORD(wp);

            return DefWindowProc(m_hwnd, msg, wp, lp);
        } break;

        case WM_NCCALCSIZE: {
            if (wp) {
                auto pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lp);
                pncsp->rgrc[0].left   = pncsp->rgrc[0].left   - EXTEND_SIDE;
                pncsp->rgrc[0].right  = pncsp->rgrc[0].right  + EXTEND_SIDE;
                pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom + EXTEND_SIDE;
                return 0;
            }
        } break;

        case WM_ACTIVATE: {
            MARGINS margins = { 0, 0, EXTEND_TOP, 0 };
            DwmExtendFrameIntoClientArea(m_hwnd, &margins);

            HMODULE hUser = GetModuleHandleA("user32.dll");
            if (hUser) {
                pfnSetWindowCompositionAttribute setWindowCompositionAttribute =
                    (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
                if (setWindowCompositionAttribute) {
                    ACCENT_POLICY accent = { ACCENT_ENABLE_GRADIENT, 0, 0, 0 };
                    WINDOWCOMPOSITIONATTRIBDATA data;
                    data.Attrib = WCA_ACCENT_POLICY;
                    data.pvData = &accent;
                    data.cbData = sizeof(accent);
                    setWindowCompositionAttribute(m_hwnd, &data);
                }
            }
        } break;

        case WM_NCHITTEST: {
            // if (this->isFullscreen()) return hit;
            POINT p = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };

            LRESULT hit;
            bool success = DwmDefWindowProc(m_hwnd, msg, wp, lp, &hit);

            if (!success) {
                RECT rcWindow;
                GetWindowRect(m_hwnd, &rcWindow);

                RECT rcFrame = { 0 };
                AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

                USHORT uRow = 1;
                USHORT uCol = 1;
                bool fOnResizeBorder = false;

                if (p.y >= rcWindow.top && p.y < rcWindow.top + EXTEND_SIDE) {
                    fOnResizeBorder = p.y < (rcWindow.top - rcFrame.top);
                    uRow = 0;
                } else if (p.y < rcWindow.bottom && p.y >= rcWindow.bottom - EXTEND_SIDE) {
                    uRow = 2;
                }

                if (p.x >= rcWindow.left && p.x < rcWindow.left + EXTEND_SIDE) {
                    uCol = 0;
                } else if (p.x < rcWindow.right && p.x >= rcWindow.right - EXTEND_SIDE) {
                    uCol = 2;
                }

                LRESULT hitTests[3][3] = {
                    { HTTOPLEFT, fOnResizeBorder ? HTTOP : HTCAPTION, HTTOPRIGHT },
                    { HTLEFT, HTCLIENT, HTRIGHT },
                    { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT }
                };

                hit = hitTests[uRow][uCol];
            }

            MapWindowPoints(nullptr, m_hwnd, &p, 1);
            if (Widget::s_capturingWidget) return hit;
            if (this->propagateCaptureMouse(toPoint(p))) return hit;
            Widget::s_hoveredWidget = this->propagateMouseEvent(toPoint(p), m_mousedown, 0);
            if (Widget::s_keyboardWidget) return hit;

            if (hit == HTCLIENT) hit = HTCAPTION;
            
            return hit;
        } break;

        case WM_SHOWWINDOW: {
            RECT rcClient;
            GetWindowRect(m_hwnd, &rcClient);

            SetWindowPos(m_hwnd, NULL, 
                rcClient.left, rcClient.top, 
                rcClient.right - rcClient.left, 
                rcClient.bottom - rcClient.top, 
                SWP_FRAMECHANGED);
        } break;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            auto hdc = BeginPaint(m_hwnd, &ps);
            HDC ndc;
            auto hpb = BeginBufferedPaint(hdc, &ps.rcPaint, BPBF_COMPATIBLEBITMAP, nullptr, &ndc);
            this->updateSize(ndc, { m_width, m_height });
            this->paint(ndc, &ps);
            EndBufferedPaint(hpb, true);
            EndPaint(m_hwnd, &ps);
            return 0;
        } break;

        case WM_LBUTTONDOWN: {
            Point p(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            m_mousedown = true;
            if (Widget::s_capturingWidget) {
                Widget::s_capturingWidget->m_mousedown = true;
                Widget::s_capturingWidget->mouseDown(p.X, p.Y);
            } else {
                this->propagateMouseEvent(p, true, 1);
            }
            if (!Widget::s_hoveredWidget && Widget::s_keyboardWidget) {
                Widget::s_keyboardWidget->releaseKeyboard();
                Widget::s_keyboardWidget = nullptr;
            }
        } break;

        case WM_LBUTTONUP: {
            Point p(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            m_mousedown = false;
            if (Widget::s_capturingWidget) {
                Widget::s_capturingWidget->m_mousedown = false;
                Widget::s_capturingWidget->mouseUp(p.X, p.Y);
            } else {
                this->propagateMouseEvent(p, false, 1);
            }
        } break;

        case WM_MOUSEMOVE: {
            Point p(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            m_mousedown = wp & MK_LBUTTON;
            if (Widget::s_capturingWidget) {
                Widget::s_hoveredWidget = Widget::s_capturingWidget;
                Widget::s_capturingWidget->m_mousedown = m_mousedown;
                Widget::s_capturingWidget->mouseMove(p.X, p.Y);
            } else {
                Widget::s_hoveredWidget = this->propagateMouseEvent(p, m_mousedown, 0);
            }
        } break;

        case WM_LBUTTONDBLCLK: {
            Point p(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            m_mousedown = true;
            if (Widget::s_capturingWidget) {
                Widget::s_capturingWidget->m_mousedown = true;
                Widget::s_capturingWidget->mouseDown(p.X, p.Y);
                Widget::s_capturingWidget->mouseDoubleClick(p.X, p.Y);
            } else {
                this->propagateMouseEvent(p, true, 2);
            }
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
            if (Widget::s_keyboardWidget) {
                if (wp == VK_ESCAPE) {
                    Widget::s_keyboardWidget = nullptr;
                } else {
                    Widget::s_keyboardWidget->keyDown(wp, LOBYTE(HIWORD(lp)));
                    return 0;
                }
            }
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

        case WM_KEYUP: {
            if (Widget::s_keyboardWidget) {
                Widget::s_keyboardWidget->keyUp(wp, LOBYTE(HIWORD(lp)));
            }
        } break;

        case WM_MOVE: {
            m_x = LOWORD(lp);
            m_y = HIWORD(lp);
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
    if (msg == WM_DESTROY) {
        if (hwnd == Manager::get()->getMainWindow()->getHWND()) {
            PostQuitMessage(0);
        }
        if (g_windows.count(hwnd)) {
            delete g_windows[hwnd];
        }
        return 0;
    }
    if (g_windows.count(hwnd)) {
        return g_windows.at(hwnd)->proc(msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
