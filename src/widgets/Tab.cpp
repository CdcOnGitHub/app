#include "Tab.hpp"
#include <array>

int Tab::s_pad = 15_px;
int Tab::s_height = 35_px;
int Tab::s_dot = 12_px;

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
    this->fontSize(16_px);
    this->show();
}

void Tab::updateSize(HDC hdc, SIZE size) {
    Widget::updateSize(hdc, size);
    if (m_autoresize) {
        this->resize(size.cx, Tab::s_height);
        m_autoresize = true;
    }
}

Color Tab::dot() {
    static int ix = -1;
    std::array<Color, 8> s = {
        0xFF60BDFF,
        0xFFCA60FF,
        0xFF60FF88,
        0xFFE7FF60,
        0xFF606EFF,
        0xFFFF6E60,
        0xFFA8FF60,
        0xFFFF60A8,
    };
    ix++;
    if (ix >= s.size()) {
        ix = 0;
    }
    return s[ix];
}

void Tab::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto r = this->rect();
    r.X += Tab::s_pad;
    r.Width -= 2 * Tab::s_pad;

    auto tr = r;
    tr.X += (s_height - Tab::s_dot) / 2 + Tab::s_pad;
    tr.Width -= (s_height - Tab::s_dot) / 2 + Tab::s_pad;
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
    Font font(hdc, Manager::get()->loadFont(m_font, m_fontsize));
    SolidBrush brush(m_color);
    gt.DrawString(
        toWString(m_text).c_str(), -1,
        &font,
        RectF {
            static_cast<float>(tr.X),
            static_cast<float>(tr.Y),
            0.f,
            static_cast<float>(tr.Height),
        },
        &f, &brush
    );

    auto g = Graphics(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    SolidBrush dotBrush(m_dotColor);
    g.FillEllipse(
        &dotBrush,
        Rect {
            r.X,
            r.Y + (s_height - Tab::s_dot) / 2,
           Tab::s_dot, Tab::s_dot
        }
    );

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
