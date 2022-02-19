#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App") {
    auto btn = new Button("Open");
    btn->move(50, 50);
    btn->setCallback([](auto) -> void {
        auto win = new TestWindow();
        win->show();
    });
    this->add(btn);

    auto themeLabel = new Label("You need to restart the app to apply this theme.");
    themeLabel->move(160, 90);
    themeLabel->hide();
    this->add(themeLabel);

    auto cb = new Checkbox("Light Theme", Style::id() == "light");
    cb->move(160, 50);
    cb->setCallback([this, themeLabel](auto c) -> void {
        Manager::get()->setTheme(c->checked() ? Theme::Default::Light : Theme::Default::Dark);
        themeLabel->show();
    });
    this->add(cb);
}
