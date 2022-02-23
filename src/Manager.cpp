#include "Manager.hpp"
#include "windows/Window.hpp"
#include <ShellScalingApi.h>
#include <fstream>

static Manager* g_manager = new Manager();

Manager* Manager::setupManager(HINSTANCE inst) {
    m_inst = inst;
    #ifndef NDEBUG
    if (AllocConsole()) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "CONOUT$", "w", stdout);
        freopen_s(&dummyFile, "CONIN$", "r", stdin);
    } else {
        MessageBoxA(nullptr, "Unable to attach console", "wtf", MB_ICONERROR);
    }
    #endif
    this->load();
    this->theme();
    m_dataLoaded = true;
    return this;
}

Manager* Manager::setup(HINSTANCE inst) {
    return g_manager->setupManager(inst);
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
    auto st = GdiplusStartup(&m_gdiToken, &m_gdiStartupInput, nullptr);
    if (st != Status::Ok) {
        auto err = "Unable to initialize Gdiplus! "
        "App may look disfigured or unusuable "
        "(Status Code " + std::to_string(st) + ")";
        MessageBoxA(nullptr, err.c_str(), "Warning", MB_ICONWARNING);
    }
    BufferedPaintInit();
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    BufferedPaintUnInit();
    GdiplusShutdown(m_gdiToken);
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

std::wstring Manager::fontFaceID(std::wstring const& font, int size) {
    return font + std::to_wstring(size);
}

HFONT Manager::loadFont(std::wstring const& face, int size, int style) {
    auto faceid = fontFaceID(face, size);
    if (m_fonts.count(faceid)) return m_fonts.at(faceid);
    auto font = CreateFontW(
        size,
        0, 0, 0, (style & FontStyleBold ? FW_BOLD : FW_NORMAL),
        style & FontStyleItalic,
        style & FontStyleUnderline,
        style & FontStyleStrikeout, 
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

void Manager::borderlessWindows(bool b) {
    m_borderlessWindows = b;
}

bool Manager::shouldWindowsBeBorderless() const {
    return m_borderlessWindows;
}

void Manager::updateDPI(HWND hwnd) {
    auto hdc = GetDC(hwnd);
    m_dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(hwnd, hdc);
}

int Manager::getDPI(HWND hwnd) {
    // todo: find a solution that works per-display
    if (!m_dpi) {
        auto hdc = GetDC(hwnd);
        m_dpi = GetDeviceCaps(hdc, LOGPIXELSX);
        ReleaseDC(hwnd, hdc);
    }
    return m_dpi;
}

float Manager::getDPIScale(HWND hwnd) {
    return this->getDPI() / 96.f;
}

int Manager::scale(int val) {
    return MulDiv(val, Manager::get()->getDPI(), 96);
}

POINT Manager::scale(POINT const& val) {
    return {
        MulDiv(val.x, Manager::get()->getDPI(), 96),
        MulDiv(val.y, Manager::get()->getDPI(), 96)
    };
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

int operator"" _px(unsigned long long px) {
    return Manager::scale(static_cast<int>(px));
}

float operator"" _pxf(long double px) {
    return static_cast<float>(Manager::get()->getDPIScale() * px);
}
