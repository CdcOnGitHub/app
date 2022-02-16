#pragma once

#include <Windows.h>

class Window;

constexpr static auto g_windowClassName = L"GeodeApp";

class Manager {
protected:
    HINSTANCE m_inst;
    Window* m_mainWindow;

public:
    static Manager* get();
    static Manager* setup(HINSTANCE inst);

    void run(Window* window);

    HINSTANCE inst() const;
};
