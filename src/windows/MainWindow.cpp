#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"
#include <Context.hpp>

void MainWindow::onTab(size_t id) {
    m_page->clear();
    switch (id) {
        case "general"_id: {
            auto label = new Label("Welcome to " GEODEAPP_NAME " v" GEODEAPP_VERSION);
            m_page->add(label);
        } break;

        case "tools"_id: {
            auto label = new Label("Tools:tm:");
            m_page->add(label);
        } break;

        case "settings"_id: {
            auto title = new Label("Settings");
            m_page->add(title);

            auto button = new Button("Open Test Window");
            button->bg(Style::primary());
            button->callback([](auto) -> void { new TestWindow(); });
            m_page->add(button);

            auto lightMode = new Checkbox(
                "Light Theme",
                Manager::get()->theme() == Theme::Default::Light
            );
            m_page->add(lightMode);
        } break;
    }
}

MainWindow::MainWindow() : Window("Geode App v" GEODEAPP_VERSION, 800_px, 600_px) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    auto pageLayout = new VerticalLayout();
    pageLayout->fill();
    pageLayout->align(VerticalLayout::Middle);
    m_page = new PadWidget(Tab::s_pad, pageLayout);
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
    sidebarTabs->add(new Tab("tools"_id, "Tools", Tab::Diamond));
    sidebarTabs->add(new TabSeparator());

    for (auto& c : Contexts::get()->all()) {
        auto tab = new Tab(c->name());
        tab->userData(c);
        sidebarTabs->add(tab);
    }
    auto addContextTab = new Tab("New Context", Tab::Plus);
    addContextTab->makeButton(true);
    sidebarTabs->add(addContextTab);

    sidebarTabs->add(new Pad(true));
    sidebarTabs->add(new Tab("settings"_id, "Settings", Tab::Diamond));

    sidebarBG->add(sidebarTabs);
    layout->first(sidebarBG);

    layout->min(Tab::s_pad * 2 + Tab::s_dot);
    layout->max(400_px);
    layout->moveSplit(250_px);

    this->add(layout);
}
