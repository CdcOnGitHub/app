#pragma once

#include <Windows.h>
#include <string>
#include <utils.hpp>

#define DEF_THEME_GETTER(_var_) \
    inline static Color const& _var_() { return Style::current()->m_theme.m_##_var_; }

struct Theme {
    std::string m_id;
    Color m_BG;
    Color m_sidebar;
    Color m_text;
    Color m_primary;
    Color m_secondary;
    Color m_tab;
    Color m_separator;
    Color m_hover;
    Color m_selectedTab;
    Color m_dark;
    Color m_inputBG;

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
    DEF_THEME_GETTER(sidebar);
    DEF_THEME_GETTER(text);
    DEF_THEME_GETTER(primary);
    DEF_THEME_GETTER(secondary);
    DEF_THEME_GETTER(tab);
    DEF_THEME_GETTER(separator);
    DEF_THEME_GETTER(hover);
    DEF_THEME_GETTER(selectedTab);
    DEF_THEME_GETTER(dark);
    DEF_THEME_GETTER(inputBG);

    static Style* current();
    void load(Theme const& theme);
    void load(Theme::Default theme);
};
