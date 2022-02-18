#pragma once

#include <ostream>
#include <Windows.h>

std::ostream& operator<<(std::ostream&, RECT);
std::ostream& operator<<(std::ostream&, POINT);

namespace color_utils {
    COLORREF darken(COLORREF color, int darken);
    COLORREF lighten(COLORREF color, int lighten);
}
