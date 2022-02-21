#include "utils.hpp"

std::ostream& operator<<(std::ostream& stream, RECT rect) {
    return stream
        << "left: " << rect.left
        << ", right: " << rect.right
        << ", top: " << rect.top
        << ", bottom: " << rect.bottom;
}

std::ostream& operator<<(std::ostream& stream, POINT p) {
    return stream << "x: " << p.x << ", y: " << p.y;
}

std::ostream& operator<<(std::ostream& stream, SIZE p) {
    return stream << "cx: " << p.cx << ", cy: " << p.cy;
}

COLORREF color_utils::darken(COLORREF color, int darken) {
    return RGB(GetRValue(color) - darken, GetGValue(color) - darken, GetBValue(color) - darken);
}

COLORREF color_utils::lighten(COLORREF color, int lighten) {
    return darken(color, -lighten);
}
