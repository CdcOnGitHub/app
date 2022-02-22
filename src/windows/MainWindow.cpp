#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Layout.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App", 800, 600) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    auto left = new RectWidget();
    left->color(Style::sidebar());
    auto vleft = new Tabs(new VerticalLayout());
    auto label = new Label(
        GEODEAPP_NAME " " GEODEAPP_VERSION "; Built on " __DATE__ " at " __TIME__
    );
    label->wrap(false);
    label->color(color::alpha(Style::text(), 100));
    vleft->add(label);
    vleft->add(new Tab("General", Tab::Diamond));
    vleft->add(new Tab("Advanced", Tab::Diamond));
    vleft->add(new Tab("Tools", Tab::Diamond));
    vleft->add(new TabSeparator());
    vleft->add(new Tab("Vanilla"));
    vleft->add(new Tab("Gay sex"));
    vleft->add(new Tab("Homosexual geckos"));
    vleft->add(new Tab("Femboy Hooters"));
    vleft->add(new Tab("Catgirls"));
    vleft->add(new Pad(true));
    vleft->add(new Tab("Settings", Tab::Diamond));
    left->add(vleft);
    layout->first(left);

    auto right = new VerticalLayout();
    for (int i = 0; i < 3; i++) {
        right->add(new Label("my girlfriend has " + std::to_string(i) + " bones in her body"));
    }
    auto btn = new Button("Open Test Window");
    btn->callback([](auto) -> void {
        new TestWindow();
    });
    btn->bg(Style::primary());
    right->add(btn);
    auto cbox = new Checkbox("Test Checkbox Thing");
    right->add(cbox);
    layout->second(right);

    layout->moveSplit(250_px);

    this->add(layout);
}
