#include "MainWindow.hpp"
#include <Label.hpp>
#include <Button.hpp>
#include <RectWidget.hpp>
#include <Checkbox.hpp>
#include <Tab.hpp>
#include "TestWindow.hpp"
#include <Context.hpp>
#include <SelectBox.hpp>
#include "CreateContextWindow.hpp"

MainWindow::~MainWindow() {
}

MainWindow::MainWindow() : Window("Geode App v" GEODEAPP_VERSION, 600_px, 500_px) {
    auto layout = new VerticalLayout();

    auto bottomPad = new PadWidget(Tab::s_pad, new HorizontalLayout());

    auto selectProfile = new SelectBox();
    selectProfile->drawWidth(10);
    bottomPad->add(selectProfile);

    bottomPad->add(new Pad(true));

    auto launchCurrent = new Button("Launch");
    launchCurrent->bg(Style::launch());
    bottomPad->add(launchCurrent);

    layout->add(bottomPad);

    layout->add(new Separator(false, 0_px, 1_px));
    layout->add(new Pad(true));

    auto contentLayout = new VerticalLayout();
    contentLayout->pad(Tab::s_pad);
    auto contentPad = new PadWidget(Tab::s_pad, contentLayout);

    auto checkbox = new Checkbox(
        "Gay checkbox (homosexual)",
        Manager::get()->theme() == Theme::Default::Light
    );
    checkbox->callback([](auto b) -> void {
        Manager::get()->setTheme(b->checked() ? Theme::Default::Light : Theme::Default::Dark);
    });
    contentPad->add(checkbox);

    auto input = new Input();
    input->placeHolder("Cock and balls...");
    contentPad->add(input);

    auto bigInput = new Input();
    bigInput->drawSize(30, 5);
    bigInput->limit(15);
    bigInput->placeHolder("BIGGA INPUDA");
    contentPad->add(bigInput);

    auto testButton = new Button("Open Test Window");
    testButton->callback([](auto b) -> void {
        new TestWindow();
    });
    contentPad->add(testButton);

    layout->add(contentPad);

    layout->invert();
    this->add(layout);
}
