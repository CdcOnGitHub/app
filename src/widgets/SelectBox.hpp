#pragma once

#include <Widget.hpp>
#include <Layout.hpp>

class SelectBox : public TextWidget {
protected:
    std::vector<std::string> m_options;
    size_t m_selected = 0;
    int m_drawWidth = 0;

public:
    SelectBox(std::initializer_list<std::string> const&);
    SelectBox(std::vector<std::string> const&);
    SelectBox();

    void drawWidth(int mw);
    
    void updateSize(HDC hdc, SIZE) override;
    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    bool wantsMouse() const override;
    HCURSOR cursor() const;
};
