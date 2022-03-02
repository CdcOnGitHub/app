#include "Style.hpp"
#include <iostream>
#include "Manager.hpp"

#define STATIC_RETURN(...) { static Theme r = __VA_ARGS__; return r; }

Style::Style() {
    this->load(Theme::get(Manager::get()->theme()));
}

Style* Style::current() {
    static auto inst = new Style();
    return inst;
}

void Style::load(Theme const& t) {
    m_theme = t;
}

Theme& Theme::get(Theme::Default d) {
    switch (d) {
        default:
        case Default::Light: STATIC_RETURN({
            "light",
            "Segoe UI",
            Color(237, 236, 236),
            Color(200, 200, 200),
            Color(0, 0, 0),
            Color(111, 125, 170),
            Color(170, 111, 152),
            Color(55, 55, 80),
            Color(75, 0, 0, 0),
            Color(55, 0, 0, 0),
            Color(255, 255, 255),
            Color(180, 180, 180),
            Color(255, 255, 255),
            Color(155, 155, 80),
            Color(155, 80, 80),
            Color(125, 235, 125),
            Color(238, 237, 236),
            Color(95, 138, 137, 255),
            -50,
            -50,
            30,
            true
        });

        case Default::Dark: STATIC_RETURN({
            "dark",
            "Segoe UI",
            Color(46, 47, 48),
            Color(25, 24, 30),
            Color(255, 255, 255),
            Color(111, 125, 170),
            Color(170, 111, 152),
            Color(255, 255, 180),
            Color(25, 255, 255, 255),
            Color(55, 255, 255, 255),
            Color(38, 36, 45),
            Color(109, 104, 130),
            Color(56, 57, 58),
            Color(155, 155, 80),
            Color(155, 80, 80),
            Color(35, 125, 35),
            Color(64, 66, 68),
            Color(55, 138, 137, 255),
            35,
            -10,
            20,
            true
        });

        case Default::Flat: STATIC_RETURN({
            "flat",
            "Segoe UI",
            Color(38, 36, 45),
            Color(25, 24, 30),
            Color(255, 255, 255),
            Color(111, 125, 170),
            Color(170, 111, 152),
            Color(255, 255, 180),
            Color(25, 255, 255, 255),
            Color(55, 255, 255, 255),
            Color(38, 36, 45),
            Color(109, 104, 130),
            Color(25, 24, 30),
            Color(155, 155, 80),
            Color(155, 80, 80),
            Color(65, 165, 65),
            Color(64, 66, 68),
            Color(55, 138, 137, 255),
            35,
            -10,
            0,
            false
        });

        case Default::Gay: STATIC_RETURN({
            "gay", // id
            "Segoe UI", // font
            Color(255, 0, 220), // bg
            Color(25, 24, 30), // sidebar
            Color(0, 255, 0), // text
            Color(111, 125, 170), // primary
            Color(170, 111, 152), // secondary
            Color(255, 255, 180), // tab
            Color(85, 0, 255, 0), // separator
            Color(55, 255, 255, 255), // hover
            Color(38, 36, 45), // selected tab
            Color(109, 104, 130), // dark
            Color(25, 24, 30), // input
            Color(155, 155, 80), // warning
            Color(155, 80, 80), // error
            Color(75, 185, 75), // launch
            Color(77, 79, 255), // button
            Color(55, 138, 137, 255), // select
            35, // button border
            -10, // button press
            -80, // button gradient
            true, // use borders
        });
    }
}
