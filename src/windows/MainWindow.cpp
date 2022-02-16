#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>

MainWindow::MainWindow() : Window("Geode App") {
    auto label = new Label("Hello World");
    label->move(40, 40);
    label->resize(100, 50);
    label->setFont("Comic Sans MS");
    this->add(label);

    auto btn = new Button("Acquire Estrogen");
    btn->move(80, 100);
    btn->resize(150, 30);
    btn->setFont("Segoe UI", 18);
    this->add(btn);
}
