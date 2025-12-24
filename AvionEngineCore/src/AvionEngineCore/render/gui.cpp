#include "../includes/AvionEngineCore/render/gui.hpp"

Gui::Gui(GLFWwindow* window): window_(window), io_(ImGui::GetIO()) {}

void Gui::Init() {
    IMGUI_CHECKVERSION();
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();
}

void Gui::Frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::CleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::CustomWindow() {
    ImGui::Begin("Hello, world!");
    bool is_checked = false;
    ImGui::Text("This text");
    ImGui::Checkbox("Window", &is_checked);
    ImGui::Button("Button");
    ImGui::End();
}   