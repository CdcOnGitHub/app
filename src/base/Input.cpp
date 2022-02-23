#include "Input.hpp"

Input::Input() {
    this->text("");
    this->font("Segoe UI");
    this->color(Style::text());
    this->autoResize();
    this->show();
}

void Input::placeHolder(std::string const& text) {
    m_placeHolder = text;
    this->update();
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
        std::string mtext = "";
        for (size_t i = 0; i < m_drawLineCount; i++) {
            mtext += std::string(m_drawCharCount, 'w') + '\n';
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
    Widget::paint(hdc, ps);
}

void Input::keyDown(size_t key, size_t scanCode) {
    // if (key == VK_BACK) {
    //     m_text.pop_back();
    //     return;
    // }
    // HKL list[2]; 
    // GetKeyboardLayoutList(2, list); 
    // BYTE keyboardState[256];
    // wchar_t buffer[1024];
    // auto size = ToUnicodeEx(
    //     static_cast<UINT>(key),
    //     static_cast<UINT>(scanCode),
    //     keyboardState,
    //     buffer, 1024,
    //     0, list[1]
    // );
    // if (size > 0) {
    //     m_text += std::wstring(buffer);
    // }
}

void Input::click() {
    this->captureKeyboard();
}
