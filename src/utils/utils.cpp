#include "utils.hpp"

// https://stackoverflow.com/questions/67052759/c-gdi-how-to-draw-rectangle-with-border-radius
void GetRoundRectPath(GraphicsPath *pPath, Rect r, int dia) {
    // diameter can't exceed width or height
    if(dia > r.Width)    dia = r.Width;
    if(dia > r.Height)    dia = r.Height;

    // define a corner 
    Rect Corner(r.X, r.Y, dia, dia);

    // begin path
    pPath->Reset();

    // top left
    pPath->AddArc(Corner, 180, 90);    

    // tweak needed for radius of 10 (dia of 20)
    if(dia == 20)
    {
        Corner.Width += 1; 
        Corner.Height += 1; 
        r.Width -=1; r.Height -= 1;
    }

    // top right
    Corner.X += (r.Width - dia - 1);
    pPath->AddArc(Corner, 270, 90);    
    
    // bottom right
    Corner.Y += (r.Height - dia - 1);
    pPath->AddArc(Corner,   0, 90);    
    
    // bottom left
    Corner.X -= (r.Width - dia - 1);
    pPath->AddArc(Corner,  90, 90);

    // end path
    pPath->CloseFigure();
}

void DrawRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width) {
    int dia = 2*radius;

    // set to pixel mode
    int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

    // define the pen
    Pen pen(color, 1);    
    pen.SetAlignment(PenAlignmentCenter);

    // get the corner path
    GraphicsPath path;

    // get path
    GetRoundRectPath(&path, r, dia);

    // draw the round rect
    pGraphics->DrawPath(&pen, &path);

    // if width > 1
    for (int i = 1; i < width; i++) {
        // left stroke
        r.Inflate(-1, 0);
        // get the path
        GetRoundRectPath(&path, r, dia);
            
        // draw the round rect
        pGraphics->DrawPath(&pen, &path);

        // up stroke
        r.Inflate(0, -1);

        // get the path
        GetRoundRectPath(&path, r, dia);
            
        // draw the round rect
        pGraphics->DrawPath(&pen, &path);
    }

    // restore page unit
    pGraphics->SetPageUnit((Unit)oldPageUnit);
}

void FillRoundRect(Graphics* pGraphics, Rect r, Color const& color, int radius, int width) {
    int dia = 2*radius;

    // set to pixel mode
    int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

    SolidBrush brush(color);

    // get the corner path
    GraphicsPath path;

    // get path
    GetRoundRectPath(&path, r, dia);

    // draw the round rect
    pGraphics->FillPath(&brush, &path);

    // if width > 1
    for (int i = 1; i < width; i++) {
        // left stroke
        r.Inflate(-1, 0);
        // get the path
        GetRoundRectPath(&path, r, dia);
            
        // draw the round rect
        pGraphics->FillPath(&brush, &path);

        // up stroke
        r.Inflate(0, -1);

        // get the path
        GetRoundRectPath(&path, r, dia);
            
        // draw the round rect
        pGraphics->FillPath(&brush, &path);
    }

    // restore page unit
    pGraphics->SetPageUnit((Unit)oldPageUnit);
}

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

std::wstring toWString(std::string const& str) {
    if (str.empty()) {
        return std::wstring();
    }

    size_t charsNeeded = MultiByteToWideChar(
        CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0
    );
    if (charsNeeded == 0) {
        return std::wstring();
    }

    std::vector<wchar_t> buffer(charsNeeded);
    int charsConverted = MultiByteToWideChar(
        CP_UTF8, 0, str.data(), (int)str.size(), &buffer[0],
        static_cast<int>(buffer.size())
    );
    if (charsConverted == 0) {
        return std::wstring();
    }

    return std::wstring(&buffer[0], charsConverted);
}

RectF toRectF(Rect const& r) {
    return {
        static_cast<float>(r.X),
        static_cast<float>(r.Y),
        static_cast<float>(r.Width),
        static_cast<float>(r.Height),
    };
}

RECT toRECT(Rect const& r) {
    return {
        r.X,
        r.Y,
        r.X + r.Width,
        r.Y + r.Height,
    };
}

Point toPoint(POINT const& p) {
    return { p.x, p.y };
}

Color color_utils::darken(Color const& color, BYTE darken) {
    return {
        color.GetA(),
        static_cast<BYTE>(color.GetR() - darken),
        static_cast<BYTE>(color.GetG() - darken),
        static_cast<BYTE>(color.GetB() - darken)
    };
}

Color color_utils::lighten(Color const& color, BYTE lighten) {
    return darken(color, -lighten);
}
