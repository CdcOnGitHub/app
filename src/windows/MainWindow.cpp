#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"

void MainWindow::onTab(size_t id) {
    m_page->clear();
    switch (id) {
        case "general"_id: {
            auto label = new Label("Welcome to " GEODEAPP_NAME " v" GEODEAPP_VERSION);
            m_page->add(label);
        } break;

        case "advanced"_id: {
            auto label = new Label("Advanced:tm:");
            m_page->add(label);
        } break;
    }
}

MainWindow::MainWindow() : Window("Geode App v" GEODEAPP_VERSION, 800_px, 600_px) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    m_page = new PadWidget(Tab::s_pad, new VerticalLayout());
    layout->second(m_page);

    auto sidebarBG = new RectWidget();
    sidebarBG->color(Style::sidebar());

    auto sidebarTabs = new Tabs(new VerticalLayout());
    sidebarTabs->onSelect(std::bind(&MainWindow::onTab, this, std::placeholders::_1));

    auto labelPad = new PadWidget(Tab::s_pad);
    auto label = new Label(
        GEODEAPP_NAME " v" GEODEAPP_VERSION
    );
    label->wrap(false);
    labelPad->add(label);
    sidebarTabs->add(labelPad);

    sidebarTabs->add(new Tab("general"_id, "General", Tab::Diamond));
    sidebarTabs->add(new Tab("advanced"_id, "Advanced", Tab::Diamond));
    sidebarTabs->add(new Tab("tools"_id, "Tools", Tab::Diamond));
    sidebarTabs->add(new TabSeparator());
    sidebarTabs->add(new Tab("Vanilla"));
    sidebarTabs->add(new Tab("Gay sex"));
    sidebarTabs->add(new Tab("Homosexual geckos"));
    sidebarTabs->add(new Tab("Femboy Hooters"));
    sidebarTabs->add(new Tab("Catgirls"));
    sidebarTabs->add(new Pad(true));
    sidebarTabs->add(new Tab("Settings", Tab::Diamond));

    sidebarBG->add(sidebarTabs);
    layout->first(sidebarBG);

    layout->min(Tab::s_pad * 2 + Tab::s_dot);
    layout->max(400_px);
    layout->moveSplit(250_px);

    this->add(layout);
}
