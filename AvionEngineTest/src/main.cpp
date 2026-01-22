#include "AvionEngineCore/core/window.hpp"

const char* name_window = "Test Engine";

int main() {
    std::cout << "Avion Engine v. 0.0.1" << std::endl;

    avion::core::Window window(name_window, 1280, 1024);

    window.Init();
    window.Update();

    return 0;
}