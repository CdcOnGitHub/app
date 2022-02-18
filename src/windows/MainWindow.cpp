#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App") {
    auto btn = new Button("Open");
    btn->move(50, 50);
    btn->setCallback([](auto) -> void {
        auto win = new TestWindow();
        win->show();
    });
    this->add(btn);
}
