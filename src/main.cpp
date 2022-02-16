#include "Manager.hpp"
#include "windows/Window.hpp"
#include "windows/MainWindow.hpp"
#include <iostream>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Manager::setup(hInstance);    
    Manager::get()->run(new MainWindow());
    return 0;
}
