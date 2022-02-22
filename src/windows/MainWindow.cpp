#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Layout.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App v" GEODEAPP_VERSION, 800, 600) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    auto left = new RectWidget();
    left->color(Style::sidebar());
    auto vleft = new Tabs(new VerticalLayout());
    auto labelPad = new PadWidget(Tab::s_pad);
    auto label = new Label(
        GEODEAPP_NAME " v" GEODEAPP_VERSION
    );
    label->wrap(false);
    labelPad->add(label);
    vleft->add(labelPad);
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

    layout->min(Tab::s_pad * 2 + Tab::s_dot);
    layout->max(400_px);
    layout->moveSplit(250_px);

    this->add(layout);
}
