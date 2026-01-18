#include "AvionEngineCore/core/window.hpp"

const char* name_window = "Test Engine";

int main() {
    std::cout << "Test Engine" << std::endl;

    avion::core::Window window(name_window, 800, 600);

    window.Init();
    window.Update();

    return 0;
}