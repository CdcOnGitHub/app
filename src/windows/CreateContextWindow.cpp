#include "CreateContextWindow.hpp"
#include <Input.hpp>

CreateContextWindow::CreateContextWindow() : Window("New Context", true, 400_px, 320_px) {
    auto padding = new PadWidget(Tab::s_pad);

    auto layout = new VerticalLayout();
    layout->fill();

    auto title = new Label("Create New Context");
    title->fontSize(30_px);
    title->style(FontStyleBold);
    layout->add(title, VerticalLayout::Middle);

    auto input = new Input();
    input->wrap(false);
    input->placeHolder("Context Name...");
    layout->add(input);

    padding->add(layout);

    this->add(padding);
}
