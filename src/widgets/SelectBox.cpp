#pragma once

#include "SelectBox.hpp"

SelectBox::SelectBox(std::initializer_list<std::string> const& v) {
    m_options = v;
    this->autoResize();
    this->show();
}

SelectBox::SelectBox(std::vector<std::string> const& v) {
    m_options = v;
    this->autoResize();
    this->show();
}

SelectBox::SelectBox() : SelectBox({}) {}

void SelectBox::drawWidth(int mw) {
    m_drawWidth = mw;
    this->update();
}

void SelectBox::updateSize(HDC hdc, SIZE available) {
    if (m_autoresize) {
        auto r = this->measureText(hdc, std::wstring(m_drawWidth, 'w'), available, StringFormat());
        this->resize(static_cast<int>(r.Width) + Button::s_sidePad * 2, static_cast<int>(r.Height) + Button::s_topPad * 2);
        m_autoresize = true;
    }
    Widget::updateSize(hdc, available);
}

void SelectBox::paint(HDC hdc, PAINTSTRUCT* ps) {

}

bool SelectBox::wantsMouse() const {
    return true;
}

HCURSOR SelectBox::cursor() const {
    return Manager::cursor(IDC_HAND);
}
