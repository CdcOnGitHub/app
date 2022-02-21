#include "Manager.hpp"
#include "windows/Window.hpp"
#include <ShellScalingApi.h>
#include <fstream>

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
    g_manager->load();
    g_manager->theme();
    g_manager->m_dataLoaded = true;
    return g_manager;
}

void Manager::setTheme(Theme::Default theme) {
    m_theme = theme;
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
    this->save();
}

void Manager::load() {
    wchar_t buff[MAX_PATH];
    if (GetModuleFileNameW(nullptr, buff, sizeof buff)) {
        std::filesystem::path path = buff;
        path = path.parent_path() / "settings";
        if (std::filesystem::exists(path)) {
            int version = 0;
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open()) return;
            file.read(reinterpret_cast<char*>(&version), sizeof version);
            if (version != 1) return;
            int theme = 0;
            file.read(reinterpret_cast<char*>(&theme), sizeof theme);
            m_theme = static_cast<Theme::Default>(theme);
        }
    }
}

void Manager::save() {
    wchar_t buff[MAX_PATH];
    if (GetModuleFileNameW(nullptr, buff, sizeof buff)) {
        std::filesystem::path path = buff;
        path = path.parent_path() / "settings";
        std::ofstream file(path, std::ios_base::out | std::ios::binary);
        if (!file.is_open()) return;
        int version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof version);
        file.write(reinterpret_cast<const char*>(&m_theme), sizeof m_theme);
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
        size,
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

int Manager::acquireWindowClassID() {
    auto id = 0;
    while (m_classIDs.count(id)) id++;
    m_classIDs.insert(id);
    return id;
}

void Manager::relinquishWindowClassID(int id) {
    m_classIDs.erase(id);
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

int Manager::scale(int val) {
    return MulDiv(val, Manager::get()->getDPI(), 96);
}

Theme::Default Manager::theme() {
    if (m_dataLoaded) {
        return m_theme;
    }
    HKEY key;
    if (RegOpenKeyA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        &key
    ) != ERROR_SUCCESS) {
        return m_theme;
    }
    DWORD value;
    DWORD size = sizeof value;
    if (RegGetValueA(key, nullptr, nullptr, RRF_RT_REG_DWORD, 0, &value, &size) == ERROR_SUCCESS) {
        m_theme = value ? Theme::Default::Light : Theme::Default::Dark;
    }
    return m_theme;
}

HCURSOR Manager::loadCursor(LPTSTR c) {
    if (!m_cursors.count(c)) {
        m_cursors[c] = LoadCursor(nullptr, c);
    }
    return m_cursors.at(c);
}

HCURSOR Manager::cursor(LPTSTR c) {
    return Manager::get()->loadCursor(c);
}
