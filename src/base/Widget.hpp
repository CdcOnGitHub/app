#pragma once

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>
#include "../Manager.hpp"
#include <utils.hpp>
#include <Style.hpp>
#include <string>
#include <dwmapi.h>
#include <functional>

class Window;

class Widget {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    bool m_autoresize = false;
    bool m_visible = false;
    bool m_hovered = false;
    bool m_mousedown = false;
    bool m_tabbed = false;
    bool m_keyboardFocused = false;
    Widget* m_parent = nullptr;
    Window* m_window = nullptr;
    std::vector<Widget*> m_children;
    const char* m_typeName = "Widget";
    std::string m_name = "";
    void* m_userData = nullptr;
    static Widget* s_hoveredWidget;
    static Widget* s_capturingWidget;
    static Widget* s_keyboardWidget;

    void updatePosition();
    void setWindow(Window*);
    Widget* propagateMouseEvent(Point& p, bool down, int clickCount);
    bool propagateTabEvent(int& index, int target);
    bool propagateCaptureMouse(Point& p);
    void propagateFocusEvent(bool focused);
    void captureMouse();
    void releaseMouse();
    void captureKeyboard();
    void releaseKeyboard();

    friend class Window;

public:
    virtual ~Widget();

    virtual void paint(HDC hdc, PAINTSTRUCT* ps);
    virtual void updateSize(HDC hdc, SIZE available);

    Widget* getParent() const;
    std::vector<Widget*> getChildren() const;

    virtual void add(Widget* child);
    virtual void remove(Widget* child, bool release = true);
    virtual void clear();
    virtual void resize(int width, int height);
    virtual void autoResize();
    virtual void move(int x, int y);
    virtual void show(bool v = true);
    void hide();
    virtual void update();
    virtual void enter();
    virtual void leave();
    void tabEnter();
    void tabLeave();
    virtual void click();
    virtual void mouseDown(int x, int y);
    virtual void mouseDoubleClick(int x, int y);
    virtual void mouseUp(int x, int y);
    virtual void mouseMove(int x, int y);
    virtual bool wantsMouse() const;
    virtual void keyDown(size_t key, size_t scanCode);
    virtual void keyUp(size_t key, size_t scanCode);
    virtual void windowFocused(bool focus);
    virtual void keyboardCaptured(bool captured);
    virtual const char* type() const;
    void name(std::string const& name);
    std::string const& name();
    
    void userData(void*);
    void* userData() const;

    int x() const;
    int y() const;
    int width() const;
    int height() const;
    bool visible() const;

    virtual HCURSOR cursor() const;

    Point offset() const;
    Rect rect() const;
    Size size() const;
};

class ColorWidget : public Widget {
protected:
    Color m_color;

public:
    virtual void color(Color color);
    Color color() const;
};

class TextWidget : public ColorWidget {
protected:
    std::wstring m_text;
    std::wstring m_font;
    int m_fontSize = 18_px;
    bool m_wordWrap = true;
    int m_style = 0;

    RectF measureText(
        HDC hdc,
        std::wstring const& text,
        std::wstring const& font,
        int fontSize,
        int style,
        SIZE const& available,
        StringFormat const& format
    );
    RectF measureText(
        HDC hdc,
        std::wstring const& text,
        SIZE const& available,
        StringFormat const& format
    );
    RectF measureText(HDC hdc, SIZE const& available, StringFormat const& format = StringFormat());

    void paintText(
        HDC hdc,
        std::wstring const& text,
        std::wstring const& font,
        int fontSize,
        int style,
        Color const& color,
        Rect const& drawRect,
        StringFormat const& format
    );
    void paintText(
        HDC hdc,
        std::wstring const& text,
        int style,
        Color const& color,
        Rect const& drawRect
    );
    void paintText(
        HDC hdc,
        std::wstring const& text,
        Rect const& drawRect,
        StringFormat const& format
    );
    void paintText(HDC hdc, Rect const& drawRect, StringFormat const& format = StringFormat());

public:
    virtual void text(std::string const& text);
    virtual void text(std::wstring const& text);
    std::wstring text() const;

    void paint(HDC hdc, PAINTSTRUCT* ps) override;

    void font(std::string const& font);
    void font(std::wstring const& font);
    virtual void font(std::string const& font, int size);
    virtual void font(std::wstring const& font, int size);
    virtual void fontSize(int size = 18_px);
    void wrap(bool);
    void style(int style);
};
