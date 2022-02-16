#include "Manager.hpp"

static Manager* g_manager = new Manager();

Manager* Manager::setup(HINSTANCE inst) {
    g_manager->m_inst = inst;
    if (AllocConsole()) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "CONOUT$", "w", stdout);
        freopen_s(&dummyFile, "CONIN$", "r", stdin);
    } else {
        MessageBoxA(nullptr, "Unable to attach console", "wtf", MB_ICONERROR);
    }
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

HFONT Manager::loadFont(std::string const& face, int size) {
    if (m_fonts.count(face)) return m_fonts.at(face);
    auto font = CreateFontA(
        size, 0, 0, 0, FW_NORMAL, false, false, false, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH, face.c_str()
    );
    if (!font) {
        throw std::runtime_error("Unable to create font");
    }
    this->m_fonts[face] = font;
    return font;
}
