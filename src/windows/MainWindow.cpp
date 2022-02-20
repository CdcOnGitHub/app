#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Layout.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App") {
    auto vlayout = new VerticalLayout();


    auto layout = new HorizontalLayout();
    layout->pad(5);

    auto btn = new Button("Open");
    btn->setCallback([](auto) -> void {
        auto win = new TestWindow();
        win->show();
    });
    layout->add(btn);

    auto themeLabel = new Label("You need to restart the app to apply this theme.");
    themeLabel->hide();
    layout->add(themeLabel, HorizontalLayout::End);

    auto cb = new Checkbox("Light Theme", Style::id() == "light");
    cb->setCallback([this, themeLabel](auto c) -> void {
        Manager::get()->setTheme(c->checked() ? Theme::Default::Light : Theme::Default::Dark);
        themeLabel->show();
    });
    layout->add(cb, HorizontalLayout::Middle);

    vlayout->add(layout);


    vlayout->add(new Pad(true));


    auto layout2 = new HorizontalLayout();

    auto text1 = new Label("automatic");
    layout2->add(text1);

    layout2->add(new Pad(true));
    layout2->add(new Pad(true));

    auto text2 = new Label("padding");
    layout2->add(text2);

    vlayout->add(layout2);


    vlayout->add(new Pad(true));


    auto bottomLayout = new HorizontalLayout();

    bottomLayout->add(new Pad(true));
    auto bottomText = new Label("bottom text");
    bottomLayout->add(bottomText);
    bottomLayout->add(new Pad(true));

    vlayout->add(bottomLayout);

    this->add(vlayout);
}
