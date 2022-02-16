#include "Manager.hpp"
#include "Window.hpp"

static Manager* g_manager = new Manager;

Manager* Manager::setup(HINSTANCE inst) {
    WNDCLASSEXW wcex;

    wcex.cbSize         = sizeof(WNDCLASSEXW);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = Window::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = Manager::get()->inst();
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = g_windowClassName;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) {
        return g_manager;
    }

    g_manager->m_inst = inst;
    return g_manager;
}

Manager* Manager::get() {
    return g_manager;
}

void Manager::run(Window* window) {
    m_mainWindow = window;
    Window::run();
}

HINSTANCE Manager::inst() const {
    return m_inst;
}
