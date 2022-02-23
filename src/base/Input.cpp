#include "Input.hpp"

Input::Input() {
    this->text("");
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Input::placeHolder(std::wstring const& text) {
    m_placeHolder = text;
    this->update();
}

void Input::placeHolder(std::string const& text) {
    this->placeHolder(toWString(text));
}

bool Input::wantsMouse() const {
    return true;
}

HCURSOR Input::cursor() const {
    return Manager::cursor(IDC_IBEAM);
}

void Input::drawSize(size_t characters, size_t lines) {
    m_drawCharCount = characters;
    m_drawLineCount = lines;
    this->update();
}

void Input::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        std::wstring mtext = L"";
        for (size_t i = 0; i < m_drawLineCount; i++) {
            mtext += std::wstring(m_drawCharCount, 'w') + L'\n';
        }
        mtext.pop_back();
        auto r = this->measureText(hdc, mtext, available, StringFormat());
        this->resize(static_cast<int>(r.Width) + 10_px, static_cast<int>(r.Height) + 10_px);
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void Input::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    FillRoundRect(
        &g, r, Style::inputBG(),
        Button::s_rounding * 2, Button::s_rounding * 4
    );

    r.X += 5_px;
    r.Y += 5_px;
    r.Width -= 10_px;
    r.Height -= 10_px;
    if (!m_text.size() && !m_keyboardFocused) {
        this->paintText(
            hdc, m_placeHolder, FontStyleItalic,
            color::alpha(m_color, 150), r
        );
    } else {
        this->paintText(hdc, r);
    }
    if (m_keyboardFocused) {
        SolidBrush selectBrush(Style::text());
        if (m_cursorStart - m_cursorEnd) {
            auto textTo = m_text.substr(0, m_cursorStart);
            auto p = this->measureText(
                hdc, textTo, { r.Width, r.Height }, StringFormat()
            );
            auto textFrom = m_text.substr(0, m_cursorEnd);
            auto p2 = this->measureText(
                hdc, textFrom, { r.Width, r.Height }, StringFormat()
            );
            g.FillRectangle(
                &selectBrush,
                RectF {
                    r.X + p.X + p.Width, r.Y + p.Y,
                    p2.X, p.Height
                }
            );
        } else {
            auto textTo = m_text.substr(0, m_cursorStart);
            auto p = this->measureText(
                hdc, textTo, { r.Width, r.Height }, StringFormat()
            );
            g.FillRectangle(
                &selectBrush,
                RectF {
                    r.X + p.Width, static_cast<REAL>(r.Y),
                    1.0_pxf, p.Height
                }
            );
        }
    }
    Widget::paint(hdc, ps);
}

void Input::moveCursorBy(int pos) {
    m_cursorStart += pos;
    if (m_cursorStart <= 0) m_cursorStart = 0;
    if (m_cursorStart > m_text.size()) m_cursorStart = m_text.size();
    if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        m_cursorEnd = m_cursorStart;
    }
    this->update();
}

void Input::moveCursorTo(size_t pos) {
    m_cursorStart = pos;
    if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        m_cursorEnd = m_cursorStart;
    }
    this->update();
}

size_t Input::selected() const {
    return labs(static_cast<long>(m_cursorStart - m_cursorEnd));
}

bool Input::eraseSelection() {
    if (this->selected()) {
        if (m_cursorStart > m_cursorEnd) {
            m_text.erase(m_cursorEnd, m_cursorStart);
        } else {
            m_text.erase(m_cursorStart, m_cursorEnd);
        }
        this->moveCursorTo(m_cursorStart);
        return true;
    }
    return false;
}

void Input::keyDown(size_t key, size_t scanCode) {
    if (key == VK_BACK) {
        if (m_text.size()) {
            if (!this->eraseSelection() && m_cursorStart > 0) {
                m_text.erase(m_cursorStart - 1, 1);
                this->moveCursorBy(-1);
            }
        }
        return this->update();
    }
    if (key == VK_DELETE) {
        if (m_text.size()) {
            if (!this->eraseSelection() && m_cursorStart > 0) {
                m_text.erase(m_cursorStart - 1, 1);
                this->moveCursorBy(-1);
            }
        }
        return this->update();
    }
    if (key == VK_LEFT) {
        return this->moveCursorBy(-1);
    }
    if (key == VK_RIGHT) {
        return this->moveCursorBy(1);
    }
    if (GetKeyState(VK_CONTROL) & 0x8000) {
        if (key == 'A') {
            m_cursorStart = 0;
            m_cursorEnd = m_text.size();
        }
        return this->update();
    }
    HKL list[2]; 
    GetKeyboardLayoutList(2, list); 
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);
    wchar_t buffer[1024];
    auto size = ToUnicodeEx(
        static_cast<UINT>(key),
        static_cast<UINT>(scanCode),
        keyboardState,
        buffer, 1024,
        0, list[1]
    );
    if (size > 0) {
        this->eraseSelection();
        m_text.insert(m_cursorStart, std::wstring(buffer));
        this->moveCursorBy(1);
        this->update();
    }
}

void Input::click() {
    this->captureKeyboard();
}
