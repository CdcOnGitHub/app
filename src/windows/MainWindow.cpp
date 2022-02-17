#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>

MainWindow::MainWindow() : Window("Geode App") {
    auto label = new Label("Hello World longer text and stuff yeahh");
    label->move(40, 40);
    label->setFont("Comic Sans MS");
    this->add(label);

    auto label2 = new Label("Awesome another label");
    label2->move(40, 220);
    label2->setColor(RGB(255, 50, 80));
    label2->setFontSize(35);
    this->add(label2);

    auto btn = new Button("Acquire Estrogen");
    btn->move(80, 100);
    btn->resize(250, 10);
    btn->setFont("Segoe UI", 18);
    btn->setCallback([](auto b) -> void { std::cout << "hello from " << b->getText() << "\n"; });
    this->add(btn);

    auto btn2 = new Button("Become GAY");
    btn2->move(380, 100);
    btn2->setCallback([](auto b) -> void { std::cout << "GAY from " << b->getText() << "\n"; });
    this->add(btn2);
}
