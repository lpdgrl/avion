#include "AvionEngineEditor/editor/editor_app.hpp"

int main(int argc, char** argv) 
{
    using EditorApp = avion::editor::app::EditorApp;

    const char* name_window = "Avion Editor";
    int width  = 1920;
    int height = 1080;

    EditorApp editor(name_window, width, height);

    editor.Init();
    editor.Run();

    return 0;
}
