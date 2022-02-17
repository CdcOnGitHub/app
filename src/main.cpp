#include "Manager.hpp"
#include "windows/Window.hpp"
#include "windows/MainWindow.hpp"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Manager::setup(hInstance);
    // try {
        Manager::get()->run(new MainWindow());
    // } catch(std::exception& e) {
        // std::cout << e.what() << "\n";
    // }
    return 0;
}
