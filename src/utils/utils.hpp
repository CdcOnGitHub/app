#pragma once

#include <Windows.h>
#include <ostream>
#include <string>
#include <vector>
#include <gdiplus.h>
#include <algorithm>

using namespace Gdiplus;

void GetRoundRectPath(GraphicsPath *pPath, Rect r, int dia);
void DrawRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width);
void FillRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width);
void FillRoundRect(Graphics* pGraphics, Rect r, Brush const& brush, int radius, int width);

void InitGraphics(Graphics& g);

template<typename T>
std::vector<T> reverse(std::vector<T> const& vec) {
    auto r = vec;
    std::reverse(r.begin(), r.end());
    return r;
}

std::ostream& operator<<(std::ostream&, RECT);
std::ostream& operator<<(std::ostream&, SIZE);
std::ostream& operator<<(std::ostream&, POINT);
std::ostream& operator<<(std::ostream&, Rect);
std::ostream& operator<<(std::ostream&, RectF);
std::wostream& operator<<(std::wostream&, RectF);

std::wstring toWString(std::string const& str);
RectF toRectF(Rect const& r);
RECT toRECT(Rect const& r);
Point toPoint(POINT const& p);
PointF toPointF(Rect const& p);

namespace color {
    Color darken(Color const& color, int darken);
    Color lighten(Color const& color, int lighten);
    Color alpha(Color const& color, BYTE newAlpha);
    Color alpha(Color const& color, int newAlpha);
}

constexpr size_t const_hash(const char* input) {
    return *input ? static_cast<size_t>(*input) + 33 * const_hash(input + 1) : 5381;
}
