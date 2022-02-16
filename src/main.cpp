#include "Manager.hpp"
#include "Window.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Manager::setup(hInstance);

    Manager::get()->run(new Window("Hello World!", 600, 400));

    return 0;
}
