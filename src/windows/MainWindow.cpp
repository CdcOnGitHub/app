#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>

MainWindow::MainWindow() : Window("Geode App") {
    auto label = new Label("Hello World longer text and stuff yeahh");
    label->move(40, 40);
    label->setFont("Comic Sans MS");
    this->add(label);

    auto rect2 = new RectWidget();
    rect2->move(250, 200);
    rect2->resize(90, 60);
    rect2->setColor(RGB(180, 120, 80));
    rect2->setCornerRadius(20);
    this->add(rect2);

    auto rect = new RectWidget();
    rect->move(90, 160);
    rect->resize(90, 60);
    rect->setColor(RGB(20, 120, 80));
    this->add(rect);

    auto label2 = new Label("Awesome another label");
    label2->move(0, 0);
    label2->setColor(RGB(255, 50, 80));
    label2->setFontSize(35);
    rect->add(label2);

    rect->move(150, 160);
    
    auto btn = new Button("Acquire Estrogen");
    btn->move(80, 100);
    btn->setFont("Segoe UI", 18);
    btn->setBG(RGB(111, 125, 170));
    btn->setCallback([](auto b) -> void { std::cout << "hello from " << b->getText() << "\n"; });
    this->add(btn);

    auto btn2 = new Button("Become GAY");
    btn2->move(380, 100);
    btn2->setBG(RGB(170, 111, 152));
    btn2->setCallback([](auto b) -> void { std::cout << "GAY from " << b->getText() << "\n"; });
    this->add(btn2);
}
