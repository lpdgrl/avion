#include "AvionEngineCore/render/gui.hpp"
#include "AvionEngineCore/render/object.hpp"

Gui::Gui(GLFWwindow* window): window_(window), io_(ImGui::GetIO()) {}
Gui::~Gui() {
    CleanUp();
    std::cout << "Gui is destroyed" << '\n';
}
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

void Gui::WindowLigthColor(glm::vec3& color) const {
    ImGui::Begin("Color source light");
    ImGui::Text("Settings color");
    ImGui::ColorEdit3("Color ligth", (float*)(&color));
    ImGui::End();
}

void Gui::WindowAddObject(glm::vec3& position, glm::vec3& size, glm::vec3& color, bool& state_button) const {
    ImGui::Begin("Object");
    ImGui::Text("Settings object");
    ImGui::ColorEdit3("Color", (float*)(&color));
    ImGui::Text("x            y             z");
    ImGui::InputFloat3("Position object",(float*)(&position));
    ImGui::InputFloat3("Size object", (float*)(&size));
    state_button = ImGui::Button("Add Object");
    ImGui::End();
}   

void Gui::WindowListObjects(const std::vector<ObjectId>& objects) const {
    ImGui::Begin("Objects");
    ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * objects.size());
    ImGui::BeginListBox("Objects", listbox_size);
    for (const auto& object : objects) {
        std::string num(std::to_string(object.id));
        std::string str("id");
        str +=' ' + num;
        ImGui::Selectable(str.c_str());
    }
    ImGui::EndListBox();
    ImGui::End();
}