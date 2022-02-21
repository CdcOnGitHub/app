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
    Color(255, 255, 255),
    Color(0, 0, 0),
    Color(111, 125, 170),
    Color(170, 111, 152),
    Color(55, 55, 80),
    Color(55, 55, 55),
};

Theme g_dark = {
    "dark",
    Color(0x26, 0x24, 0x2d),
    Color(255, 255, 255),
    Color(111, 125, 170),
    Color(170, 111, 152),
    Color(255, 255, 180),
    Color(100, 100, 100),
};

Theme& Theme::get(Theme::Default d) {
    switch (d) {
        case Default::Light: return g_light;
        case Default::Dark: return g_dark;
    }
    return g_dark;
}
