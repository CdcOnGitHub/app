#pragma once

#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <Style.hpp>
#include <filesystem>
#include "config.hpp"

class Window;

class Manager {
protected:
    HINSTANCE m_inst;
    Window* m_mainWindow = nullptr;
    std::unordered_map<std::wstring, HFONT> m_fonts;
    std::unordered_map<LPTSTR, HCURSOR> m_cursors;
    std::unordered_set<HMENU> m_menuIDs;
    std::unordered_set<int> m_classIDs;
    Theme::Default m_theme = Theme::Default::Dark;
    bool m_dataLoaded = false;
    int m_dpi = 0;
    ULONG_PTR m_gdiToken;
    Gdiplus::GdiplusStartupInput m_gdiStartupInput;
    
    std::wstring fontFaceID(std::wstring const& font, int size);

    Manager* setupManager(HINSTANCE inst);

public:
    static Manager* get();
    static Manager* setup(HINSTANCE inst);

    void run(Window* window);

    HFONT loadFont(std::wstring const& font, int size = 0, int style = 0);

    HMENU acquireMenuID();
    void relinquishMenuID(HMENU);

    int acquireWindowClassID();
    void relinquishWindowClassID(int id);

    Theme::Default theme();
    void setTheme(Theme::Default);

    void save();
    void load();

    Window* getMainWindow() const;
    HINSTANCE getInst() const;
    void updateDPI(HWND = nullptr);
    int getDPI(HWND = nullptr);
    float getDPIScale(HWND = nullptr);
    static int scale(int val);
    static POINT scale(POINT const& val);

    HCURSOR loadCursor(LPTSTR);
    static HCURSOR cursor(LPTSTR);
};

int operator"" _px(unsigned long long);
float operator"" _pxf(long double);
