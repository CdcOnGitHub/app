#include "CreateContextWindow.hpp"

CreateContextWindow::CreateContextWindow() : Window("New Context", true, 400_px, 320_px) {
    auto layout = new VerticalLayout();
    layout->fill();

    auto title = new Label("Create New Context");
    title->fontSize(30_px);
    title->style(FontStyleBold);
    layout->add(title, VerticalLayout::Middle);

    this->add(layout);
}
