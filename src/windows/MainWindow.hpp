#pragma once

#include "Window.hpp"
#include <Layout.hpp>

class MainWindow : public Window {
protected:
    PadWidget* m_page;

    void onTab(size_t id);

public:
    MainWindow();
};
