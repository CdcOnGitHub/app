#pragma once

#include <Windows.h>
#include <ostream>
#include <string>
#include <vector>
#include <gdiplus.h>

using namespace Gdiplus;

void GetRoundRectPath(GraphicsPath *pPath, Rect r, int dia);
void DrawRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width);
void FillRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width);

std::ostream& operator<<(std::ostream&, RECT);
std::ostream& operator<<(std::ostream&, SIZE);
std::ostream& operator<<(std::ostream&, POINT);

std::wstring toWString(std::string const& str);
RectF toRectF(Rect const& r);
RECT toRECT(Rect const& r);
Point toPoint(POINT const& p);

namespace color_utils {
    Color darken(Color const& color, BYTE darken);
    Color lighten(Color const& color, BYTE lighten);
}
