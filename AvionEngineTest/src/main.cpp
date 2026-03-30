#include "../../AvionEngineCore/includes/AvionEngineCore/core/window.hpp"
#include "../../AvionEngineCore/includes/AvionEngineCore/macro.h"

#include <unistd.h>

const char* name_window = "Test Engine";

int main(int argc, char** argv) {

    AV_LOG_INFO("Avion Engine v. 0.0.1")
    
    avion::core::Window window(name_window, 1280, 1024); 

    window.Init();
    window.Update();

    return 0;
}
