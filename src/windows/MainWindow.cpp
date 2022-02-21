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
    left->color({ 25, 24, 30 });
    auto vleft = new Tabs(new VerticalLayout());
    vleft->add(new Tab("General"));
    vleft->add(new Tab("Advanced"));
    vleft->add(new Tab("Tools"));
    vleft->add(new TabSeparator());
    vleft->add(new Tab("Vanilla"));
    vleft->add(new Tab("Gay sex"));
    vleft->add(new Tab("Homosexual geckos"));
    vleft->add(new Tab("Femboy Hooters"));
    vleft->add(new Tab("Catgirls"));
    vleft->add(new Pad(true));
    vleft->add(new Tab("Settings"));
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
    auto cbox = new Checkbox("Test");
    right->add(cbox);
    layout->second(right);

    layout->moveSplit(250_px);

    this->add(layout);
}
