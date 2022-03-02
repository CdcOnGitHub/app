#include "Input.hpp"

using namespace std::chrono_literals;

int Input::s_pad = 5_px;

Input::Input() {
    this->text("");
    this->font(Style::font());
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Input::blink() {
    m_blink = !m_blink;
    this->update();
}

void Input::keyboardCaptured(bool k) {
    if (k) {
        if (!m_blinkTimer) {
            m_blinkTimer = m_window->timer(500, std::bind(&Input::blink, this), true);
        }
    } else {
        m_window->releaseTimer(m_blinkTimer);
        m_blinkTimer = 0;
    }
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

void Input::click() {
    this->captureKeyboard();
}

void Input::limit(size_t characters) {
    m_limitCharCount = characters;
    this->update();
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
    InitGraphics(g);

    FillRoundRect(
        &g, r, 
        Style::inputBG(),
        Button::s_rounding / 2, Button::s_rounding
    );

    m_wordWrap = m_drawLineCount > 1;

    auto tr = r;

    tr.X += s_pad;
    tr.Y += s_pad;
    tr.Width -= s_pad * 2;
    tr.Height -= s_pad * 2;
    if (!m_text.size() && !m_keyboardFocused) {
        this->paintText(
            hdc, m_placeHolder, FontStyleItalic,
            color::alpha(m_color, 150), tr
        );
    } else {
        this->paintText(hdc, tr);
    }
    if (m_keyboardFocused) {
        if (m_cursorStart - m_cursorEnd) {
            SolidBrush selectBrush(Style::select());
            auto textTo = m_text.substr(0, m_cursorStart);
            auto p = this->measureText(
                hdc, textTo, { tr.Width, tr.Height }, StringFormat()
            );
            auto textFrom = m_text.substr(0, m_cursorEnd);
            auto p2 = this->measureText(
                hdc, textFrom, { tr.Width, tr.Height }, StringFormat()
            );
            auto swidth = std::min(p.Width, p2.Width);
            auto ewidth = std::max(p.Width, p2.Width);
            swidth = (swidth ? swidth - s_pad / 2.f : swidth);
            auto h = fabsf(p2.Height - p.Height);
            g.FillRectangle(
                &selectBrush,
                RectF {
                    tr.X + swidth, static_cast<REAL>(tr.Y),
                    ewidth - swidth, h ? h : static_cast<REAL>(m_fontSize)
                }
            );
        } else if (m_blink) {
            SolidBrush selectBrush(Style::text());
            auto textTo = m_text.substr(0, m_cursorStart);
            auto lines = std::count(textTo.begin(), textTo.end(), '\n');
            if (lines) {
                textTo = textTo.substr(textTo.find_last_of('\n') + 1);
                textTo = std::wstring(lines - 1, '\n') + textTo;
            }
            std::wcout << textTo << "\n";
            auto p = this->measureText(
                hdc, textTo, { tr.Width, tr.Height }, StringFormat()
            );
            if (lines) {
                tr.Y += static_cast<int>(p.Height + ((!textTo.size() || textTo.back() == L'\n') ? lines * m_fontSize : 0));
            }
            g.FillRectangle(
                &selectBrush,
                RectF {
                    tr.X + (p.Width ? p.Width - s_pad / 2.f : p.Width),
                    static_cast<REAL>(tr.Y),
                    0.5_pxf, static_cast<REAL>(m_fontSize)
                }
            );
        }
    }

    if (Style::useBorders()) {
        DrawRoundRect(
            &g, r,
            color::lighten(Style::inputBG(), Style::buttonBorder()),
            Button::s_rounding / 2, Button::s_rounding
        );
    }

    Widget::paint(hdc, ps);
}

void Input::moveCursorBy(int pos, bool shift) {
    long cstart = static_cast<long>(m_cursorStart) + pos;
    if (cstart <= 0) cstart = 0;
    if (cstart > m_text.size()) cstart = static_cast<long>(m_text.size());
    m_cursorStart = cstart;
    if (!shift) {
        m_cursorEnd = m_cursorStart;
    }
    this->update();
}

void Input::moveCursorTo(size_t pos, bool shift) {
    m_cursorStart = pos;
    if (!shift) {
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
        this->moveCursorTo(m_cursorStart, false);
        return true;
    }
    return false;
}

void Input::windowFocused(bool f) {
    this->releaseKeyboard();
}

void Input::keyDown(size_t key, size_t scanCode) {
    m_blink = true;
    if (m_blinkTimer) {
        m_window->resetTimer(m_blinkTimer);
    }
    if (key == VK_BACK) {
        if (m_text.size()) {
            if (!this->eraseSelection() && m_cursorStart > 0) {
                if (GetKeyState(VK_CONTROL) & 0x8000) {
                    bool first = true;
                    size_t to = 0;
                    for (auto ix = m_cursorStart - 1; ix > 0; ix--) {
                        if (first) {
                            first = m_text[ix] == ' ';
                        } else {
                            if (m_text[ix] == ' ') {
                                to = ix + 1;
                                break;
                            }
                        }
                    }
                    m_text.erase(m_cursorStart - (m_cursorStart - to), m_cursorStart - to);
                    this->moveCursorTo(to, false);
                } else {
                    m_text.erase(m_cursorStart - 1, 1);
                    this->moveCursorBy(-1, false);
                }
            }
        }
        return this->update();
    }
    if (key == VK_DELETE) {
        if (m_text.size()) {
            if (!this->eraseSelection() && m_cursorStart < m_text.size()) {
                if (GetKeyState(VK_CONTROL) & 0x8000) {
                    bool first = true;
                    size_t to = m_text.size();
                    for (auto ix = m_cursorStart; ix < m_text.size(); ix++) {
                        if (first) {
                            first = m_text[ix] == ' ';
                        } else {
                            if (m_text[ix] == ' ') {
                                to = ix;
                                break;
                            }
                        }
                    }
                    m_text.erase(m_cursorStart, to - m_cursorStart);
                } else {
                    m_text.erase(m_cursorStart, 1);
                }
            }
        }
        return this->update();
    }
    if (key == VK_LEFT) {
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            bool first = true;
            size_t to = 0;
            if (m_cursorStart) {
                for (auto ix = m_cursorStart - 1; ix > 0; ix--) {
                    if (first) {
                        first = m_text[ix] == ' ';
                    } else {
                        if (m_text[ix] == ' ') {
                            to = ix + 1;
                            break;
                        }
                    }
                }
            }
            return this->moveCursorTo(to, GetKeyState(VK_SHIFT) & 0x8000);
        }
        return this->moveCursorBy(-1, GetKeyState(VK_SHIFT) & 0x8000);
    }
    if (key == VK_RIGHT) {
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            bool first = true;
            size_t to = m_text.size();
            if (m_cursorStart < m_text.size()) {
                for (auto ix = m_cursorStart; ix < m_text.size(); ix++) {
                    if (first) {
                        first = m_text[ix] == ' ';
                    } else {
                        if (m_text[ix] == ' ') {
                            to = ix;
                            break;
                        }
                    }
                }
            }
            return this->moveCursorTo(to, GetKeyState(VK_SHIFT) & 0x8000);
        }
        return this->moveCursorBy(1, GetKeyState(VK_SHIFT) & 0x8000);
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
        if (m_text.size() > m_limitCharCount) {
            return this->update();
        }
        auto str = std::wstring(buffer);
        if (str == L"\r") {
            if (!m_wordWrap) return this->update();
            str = L"\n";
        }
        if (m_cursorStart > m_text.size()) m_cursorStart = 0;
        m_text.insert(m_cursorStart, str);
        this->moveCursorBy(1, false);
        this->update();
    }
}
