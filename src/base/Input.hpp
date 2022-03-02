#pragma once

#include "Widget.hpp"
#include <string>
#include <thread>
#include <mutex>

class Input : public TextWidget {
public:
    static int s_pad;

protected:
    bool m_blink = true;
    size_t m_cursorStart = 0;
    size_t m_cursorEnd = 0;
    size_t m_drawCharCount = 20;
    size_t m_drawLineCount = 1;
    size_t m_limitCharCount = 9999;
    size_t m_hScroll = 0;
    size_t m_vScroll = 0;
    std::wstring m_placeHolder = L"";
    UINT m_blinkTimer = 0;

    void blink();

public:
    Input();

    void moveCursorBy(int pos, bool shift);
    void moveCursorTo(size_t pos, bool shift);
    size_t selected() const;
    bool eraseSelection();

    bool wantsMouse() const override;
    HCURSOR cursor() const;
    void click() override;

    void windowFocused(bool) override;
    void keyboardCaptured(bool) override;

    void drawSize(size_t characters, size_t lines);
    void limit(size_t characters);
    void placeHolder(std::wstring const& text);
    void placeHolder(std::string const& text);
    
    void keyDown(size_t key, size_t scanCode) override;
    void updateSize(HDC hdc, SIZE size) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};

