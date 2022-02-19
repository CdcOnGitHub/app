#include "Style.hpp"
#include <iostream>
#include "Manager.hpp"

Style::Style() {
    this->load(Manager::get()->theme());
}

Style* Style::current() {
    static auto inst = new Style();
    return inst;
}

void Style::load(Theme const& t) {
    m_theme = t;
}

void Style::load(Theme::Default theme) {
    this->load(Theme::get(theme));
}

Theme g_light = {
    "light",
    RGB(255, 255, 255),
    RGB(0, 0, 0),
    RGB(111, 125, 170),
    RGB(170, 111, 152),
    RGB(55, 55, 80),
};

Theme g_dark = {
    "dark",
    RGB(0x26, 0x24, 0x2d),
    RGB(255, 255, 255),
    RGB(111, 125, 170),
    RGB(170, 111, 152),
    RGB(255, 255, 180),
};

Theme& Theme::get(Theme::Default d) {
    switch (d) {
        case Default::Light: return g_light;
        case Default::Dark: return g_dark;
    }
    return g_dark;
}
