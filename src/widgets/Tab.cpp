#include "Tab.hpp"
#include <array>

int Tab::s_pad = 15_px;
int Tab::s_height = 40_px;

void Tabs::add(Widget* w) {
    if (w == m_layout) Widget::add(w);
    auto t = dynamic_cast<Tab*>(w);
    if (t) {
        m_tabs.push_back(t);
    }
    m_layout->add(w);
}

Tabs::Tabs(Layout* l) {
    m_layout = l;
    this->add(m_layout);
    this->show();
}

TabSeparator::TabSeparator() {
    this->autoresize();
    this->show();
}

void TabSeparator::updateSize(HDC hdc, SIZE size) {
    Widget::updateSize(hdc, size);
    if (m_autoresize) {
        this->resize(size.cx, 20_px);
        m_autoresize = true;
    }
}

void TabSeparator::paint(HDC hdc, PAINTSTRUCT* ps) {
    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    auto r = this->rect();
    r.Y += r.Height / 2 - 1_px;
    r.Height = 2_px;
    r.X += Tab::s_pad;
    r.Width -= 2 * Tab::s_pad;
    g.FillRectangle(&SolidBrush(Style::separator()), toRectF(r));

    Widget::paint(hdc, ps);
}

Tab::Tab(std::string const& text) {
    m_dotColor = Tab::dot();
    this->text(text);
    this->color(Style::text());
    this->autoresize();
    this->show();
}

void Tab::updateSize(HDC hdc, SIZE size) {
    Widget::updateSize(hdc, size);
    if (m_autoresize) {
        this->resize(size.cx, Tab::s_height);
        m_autoresize = true;
    }
}

COLORREF Tab::dot() {
    static int ix = -1;
    std::array<COLORREF, 8> s = {
        0x0060BDFF,
        0x00CA60FF,
        0x0060FF88,
        0x00E7FF60,
        0x00606EFF,
        0x00FF6E60,
        0x00A8FF60,
        0x00FF60A8,
    };
    ix++;
    if (ix >= s.size()) {
        ix = 0;
    }
    return s[ix];
}

void Tab::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto oldFont = SelectObject(hdc, Manager::get()->loadFont(m_font, m_fontsize));

    auto r = this->rect();
    r.X += Tab::s_pad;
    r.Width -= 2 * Tab::s_pad;

    auto tr = r;
    tr.X += Tab::s_height;
    tr.Width -= Tab::s_height;
    if (m_hovered || m_selected) {
        tr.Width -= Tab::s_height;
    }
    
    Graphics gt(hdc);

    gt.SetSmoothingMode(SmoothingModeAntiAlias);
    Region reg(tr);
    gt.SetClip(&reg);
    StringFormat f;
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    gt.DrawString(
        toWString(m_text).c_str(), -1,
        &Font(hdc, Manager::get()->loadFont(m_font, m_fontsize)),
        RectF {
            static_cast<float>(tr.X),
            static_cast<float>(tr.Y),
            0.f,
            static_cast<float>(tr.Height),
        },
        &f, &SolidBrush(m_color)
    );

    auto g = Graphics(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.FillEllipse(
        &SolidBrush(Color(m_dotColor | 0xff000000)),
        Rect {
            r.X,
            r.Y + (s_height - 12_px) / 2,
           12_px, 12_px
        }
    );

    SelectObject(hdc, oldFont);

    Widget::paint(hdc, ps);
}

bool Tab::wantsMouse() const {
    return true;
}

HCURSOR Tab::cursor() const {
    return Manager::cursor(IDC_HAND);
}

void Tab::click() {
}
