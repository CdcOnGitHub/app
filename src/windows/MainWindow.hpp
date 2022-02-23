#pragma once

#include "Window.hpp"
#include <Layout.hpp>

class MainWindow : public Window {
protected:
    std::unordered_map<size_t, VerticalLayout*> m_pages;
    PadWidget* m_page;

    void onTab(size_t id);
    void createTabs();
    template<size_t ID>
    void createTab(VerticalLayout*);
    template<size_t ID>
    void createTab();

public:
    MainWindow();
    virtual ~MainWindow();
};
