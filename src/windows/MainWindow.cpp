#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Layout.hpp>
#include "TestWindow.hpp"

MainWindow::MainWindow() : Window("Geode App", 800, 600) {
    auto layout = new SplitLayout();
    layout->hideSeparatorLine();

    auto left = new RectWidget();
    left->setColor(RGB(25, 24, 30));
    layout->first(left);

    auto right = new VerticalLayout();
    
    for (int i = 0; i < 3; i++) {
        right->add(new Label("my girlfriend has " + std::to_string(i) + " bones in her body"));
    }

    layout->second(right);

    layout->moveSplit(250);

    this->add(layout);
}
