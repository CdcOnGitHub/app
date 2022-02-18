#pragma once

#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>

class Window;

class Manager {
protected:
    HINSTANCE m_inst;
    Window* m_mainWindow = nullptr;
    std::unordered_map<std::string, HFONT> m_fonts;
    std::unordered_set<HMENU> m_menuIDs;
    std::unordered_set<int> m_classIDs;
    
    std::string fontFaceID(std::string const& font, int size);

public:
    static Manager* get();
    static Manager* setup(HINSTANCE inst);

    void run(Window* window);

    HFONT loadFont(std::string const& font, int size = 0);

    HMENU acquireMenuID();
    void relinquishMenuID(HMENU);

    int acquireWindowClassID();
    void relinquishWindowClassID(int id);

    Window* getMainWindow() const;
    HINSTANCE getInst() const;
    int getDPI(HWND = nullptr);
    float getDPIScale(HWND = nullptr);
};
