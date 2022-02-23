#pragma once

#include "Widget.hpp"
#include <string>

class Input : public TextWidget {
protected:
    size_t m_cursorStart = 0;
    size_t m_cursorEnd = 0;
    size_t m_drawCharCount = 20;
    size_t m_drawLineCount = 1;
    std::wstring m_placeHolder = L"";

public:
    Input();

    void moveCursorBy(int pos);
    void moveCursorTo(size_t pos);
    size_t selected() const;
    bool eraseSelection();

    bool wantsMouse() const override;
    HCURSOR cursor() const;
    void click() override;

    void drawSize(size_t characters, size_t lines);
    void placeHolder(std::wstring const& text);
    void placeHolder(std::string const& text);
    
    void keyDown(size_t key, size_t scanCode) override;
    void updateSize(HDC hdc, SIZE size) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;
};

