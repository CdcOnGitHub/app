#pragma once

#include <Windows.h>
#include <iostream>
#include <unordered_map>

class Window;

constexpr static auto g_windowClassName = "GeodeApp";

class Manager {
protected:
    HINSTANCE m_inst;
    Window* m_mainWindow = nullptr;
    std::unordered_map<std::string, HFONT> m_fonts;

public:
    static Manager* get();
    static Manager* setup(HINSTANCE inst);

    void run(Window* window);

    HFONT loadFont(std::string const& font, int size = 0);

    Window* getMainWindow() const;
    HINSTANCE getInst() const;
};
