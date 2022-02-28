#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"
#include <Context.hpp>
#include "CreateContextWindow.hpp"

template<>
void MainWindow::createTab<"general"_id>(VerticalLayout* page) {
    auto label = new Label("Welcome to " GEODEAPP_NAME " v" GEODEAPP_VERSION);
    page->add(label);
}

template<>
void MainWindow::createTab<"tools"_id>(VerticalLayout* page) {
    auto label = new Label("Tools:tm:");
    page->add(label);
}

template<>
void MainWindow::createTab<"settings"_id>(VerticalLayout* page) {
    auto title = new Label("Settings");
    title->style(FontStyleBold);
    title->fontSize(34_px);
    page->add(title);

    page->add(new Pad(Tab::s_pad - 10_px));

    auto button = new Button("Open Test Window");
    button->bg(Style::primary());
    button->callback([](auto) -> void { new TestWindow(); });
    page->add(button);

    auto lightModeInfoBG = new RectWidget();
    lightModeInfoBG->color(color::alpha(Style::warning(), 55));
    lightModeInfoBG->cornerRadius(Button::s_rounding);
    lightModeInfoBG->hide();

    auto lightModeInfoPad = new PadWidget(Tab::s_pad);
    lightModeInfoPad->add(new Label("You need to reload the app to apply this theme."));
    lightModeInfoBG->add(lightModeInfoPad);

    page->add(lightModeInfoBG);

    auto lightMode = new Checkbox(
        "Light Theme",
        Manager::get()->theme() == Theme::Default::Light
    );
    lightMode->callback([lightModeInfoBG](auto b) -> void {
        Manager::get()->setTheme(b->checked() ? Theme::Default::Light : Theme::Default::Dark);
        lightModeInfoBG->show();
    });
    page->add(lightMode);
}

template<size_t ID>
void MainWindow::createTab() {
    auto page = new VerticalLayout();
    page->align(VerticalLayout::Start);
    page->fill();
    page->pad(5_px);
    this->createTab<ID>(page);
    m_pages.insert({ ID, page });
}

void MainWindow::createTabs() {
    this->createTab<"general"_id>();
    this->createTab<"tools"_id>();
    this->createTab<"settings"_id>();
}

void MainWindow::onTab(size_t id) {
    if (m_pages.count(id)) {
        m_page->widget(m_pages.at(id), false);
    }
}

MainWindow::~MainWindow() {
    auto old = m_page->widget(nullptr);
    for (auto& [_, p] : m_pages) {
        if (p != old) {
            delete p;
        }
    }
    m_page = nullptr;
}

MainWindow::MainWindow() : Window("Geode App v" GEODEAPP_VERSION, 800_px, 600_px) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    this->createTabs();

    m_page = new PadWidget(Tab::s_pad, nullptr);
    layout->second(m_page);

    auto sidebarBG = new RectWidget();
    sidebarBG->color(Style::sidebar());
    sidebarBG->fill();

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
    addContextTab->callback([](auto) -> void { new CreateContextWindow(); });
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
