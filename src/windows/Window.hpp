#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <Widget.hpp>

class Window : public Widget {
protected:
    struct TimerFunc {
        std::function<void()> m_func;
        int m_time;
        bool m_repeat;
    };

    int m_classID;
    HWND m_hwnd = nullptr;
    std::string m_title;
    bool m_fullscreen = false;
    int m_tabIndex = 0;
    std::unordered_map<UINT, TimerFunc> m_timerIDs;

public:
    Window(std::string const& title, bool hasParent, int width = 600_px, int height = 400_px);
    Window(std::string const& title = GEODEAPP_NAME, int width = 600_px, int height = 400_px);
    Window() = delete;
    virtual ~Window();

    void add(Widget* child) override;
    void updateWindow(RECT rc);
    void updateWindow();
    void paint(HDC, PAINTSTRUCT*) override;
    void show(bool v = true) override;
    void move(int x, int y) override;
    UINT timer(int time, std::function<void()> func, bool repeat);
    void releaseTimer(UINT id);
    void resetTimer(UINT id);

    static void updateAll();

    void center();
    void setTitle(std::string const&);
    bool isFullscreen() const;

    HWND getHWND() const;

    LRESULT proc(UINT msg, WPARAM wparam, LPARAM lparam);

    static LRESULT CALLBACK recurseWndProc(Widget*, HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
