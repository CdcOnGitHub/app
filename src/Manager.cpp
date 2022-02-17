#include "Manager.hpp"
#include "windows/Window.hpp"
#include <ShellScalingApi.h>

static Manager* g_manager = new Manager();

Manager* Manager::setup(HINSTANCE inst) {
    g_manager->m_inst = inst;
    #ifndef NDEBUG
    if (AllocConsole()) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "CONOUT$", "w", stdout);
        freopen_s(&dummyFile, "CONIN$", "r", stdin);
    } else {
        MessageBoxA(nullptr, "Unable to attach console", "wtf", MB_ICONERROR);
    }
    #endif
    return g_manager;
}

Manager* Manager::get() {
    return g_manager;
}

void Manager::run(Window* window) {
    m_mainWindow = window;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

Window* Manager::getMainWindow() const {
    return m_mainWindow;
}

HINSTANCE Manager::getInst() const {
    return m_inst;
}

std::string Manager::fontFaceID(std::string const& font, int size) {
    return font + std::to_string(size);
}

HFONT Manager::loadFont(std::string const& face, int size) {
    auto faceid = fontFaceID(face, size);
    if (m_fonts.count(faceid)) return m_fonts.at(faceid);
    auto font = CreateFontA(
        MulDiv(size, Manager::get()->getDPI(), 96),
        0, 0, 0, FW_NORMAL, false, false, false, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH, face.c_str()
    );
    if (!font) {
        throw std::runtime_error("Unable to create font");
    }
    this->m_fonts[faceid] = font;
    return font;
}

HMENU Manager::acquireMenuID() {
    HMENU id = reinterpret_cast<HMENU>(0x100);
    while (m_menuIDs.count(id)) id++;
    m_menuIDs.insert(id);
    return id;
}

void Manager::relinquishMenuID(HMENU id) {
    m_menuIDs.erase(id);
}

int Manager::getDPI(HWND hwnd) {
    if (hwnd) return GetDpiForWindow(hwnd);
    return (m_mainWindow ?
        GetDpiForWindow(m_mainWindow->getHWND()) :
        GetDpiForSystem());
}

float Manager::getDPIScale(HWND hwnd) {
    if (hwnd) return GetDpiForWindow(hwnd) / 96.f;
    return (m_mainWindow ?
        GetDpiForWindow(m_mainWindow->getHWND()) :
        GetDpiForSystem()) / 96.f;
}
