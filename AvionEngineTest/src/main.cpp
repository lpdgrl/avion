#include "AvionEngineCore/render/window.hpp"

const char* name_window = "Test Engine";

int main() {
    std::cout << "Test Engine" << std::endl;

    Window window(name_window, 800, 600);

    window.Init();
    window.Update();

    return 0;
}