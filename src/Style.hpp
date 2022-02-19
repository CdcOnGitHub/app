#pragma once

#include <Windows.h>
#include <string>

#define DEF_THEME_GETTER(_var_) \
    inline static COLORREF _var_() { return Style::current()->m_theme.m_##_var_; }

struct Theme {
    std::string m_id;
    COLORREF m_BG;
    COLORREF m_text;
    COLORREF m_primary;
    COLORREF m_secondary;
    COLORREF m_tab;

    enum class Default {
        Light, Dark,
    };

    static Theme& get(Default);
};

class Style {
protected:
    Theme m_theme;

    Style();

public:

    static std::string const& id() { return Style::current()->m_theme.m_id; }
    DEF_THEME_GETTER(BG);
    DEF_THEME_GETTER(text);
    DEF_THEME_GETTER(primary);
    DEF_THEME_GETTER(secondary);
    DEF_THEME_GETTER(tab);

    static Style* current();
    void load(Theme const& theme);
    void load(Theme::Default theme);
};
