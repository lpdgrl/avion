#include "../../AvionEngineCore/includes/AvionEngineCore/core/engine.hpp"

int main(int argc, char** argv) {

    const char* name_window = "Test Engine";
    avion::core::Engine app(name_window, 1920, 1080); 

    app.Init();
    app.Update();

    return 0;
}
