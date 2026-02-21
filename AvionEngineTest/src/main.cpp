#include "../../AvionEngineCore/includes/AvionEngineCore/core/window.hpp"
#include "../../AvionEngineCore/includes/AvionEngineCore/macro.h"

const char* name_window = "Test Engine";

int main() {
    // std::cout << "Avion Engine v. 0.0.1" << std::endl;

    AV_LOG_INFO("Avion Engine v. 0.0.1")
    AV_LOG_DEBUG("TEST")
    
    avion::core::Window window(name_window, 1280, 1024);   

    window.Init();
    window.Update();

    return 0;
}