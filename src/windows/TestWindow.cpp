#include "TestWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>

TestWindow::TestWindow() : Window("Hello from github workflows") {
    auto label = new Label("Epic app");
    label->move(40_px, 40_px);
    label->font("Comic Sans MS");
    this->add(label);

    auto rect2 = new RectWidget();
    rect2->move(250_px, 200_px);
    rect2->resize(90_px, 60_px);
    rect2->color({ 180, 120, 80 });
    rect2->cornerRadius(20_px);
    this->add(rect2);

    auto rect = new RectWidget();
    rect->move(90_px, 160_px);
    rect->resize(90_px, 60_px);
    rect->color({ 20, 120, 80 });
    this->add(rect);

    auto label2 = new Label("Awesome another label");
    label2->move(0, 0);
    label2->color({ 255, 50, 80 });
    label2->fontSize(35_px);
    rect->add(label2);

    rect->move(150, 160);
    
    auto btn = new Button("Acquire Estrogen");
    btn->move(80_px, 100_px);
    btn->font(Style::font(), 18_px);
    btn->bg({ 111, 125, 170 });
    btn->callback([](auto b) -> void { std::wcout << L"hello from " << b->text() << L"\n"; });
    this->add(btn);

    auto btn2 = new Button("Become GAY");
    btn2->move(380_px, 100_px);
    btn2->bg({ 170, 111, 152 });
    btn2->callback([](auto b) -> void { std::wcout << L"GAY from " << b->text() << L"\n"; });
    this->add(btn2);
}
