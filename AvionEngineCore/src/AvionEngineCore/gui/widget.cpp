#include "AvionEngineCore/gui/widget.hpp"
#include "AvionEngineCore/core/scene.hpp"

namespace avion::gui {

    Widget::Widget(GLFWwindow* window): window_(window), io_(ImGui::GetIO()) {}
    Widget::~Widget() {
        CleanUp();
        std::cout << "Widget is destroyed" << '\n';
    }
    void Widget::Init() {
        IMGUI_CHECKVERSION();
        io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init();
    }

    void Widget::Frame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Widget::Render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Widget::CleanUp() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Widget::WindowLigthColor(glm::vec3& position, glm::vec3& color) const {
        ImGui::Begin("Color source light");
            ImGui::Text("Position object:");
            ImGui::SliderFloat("x-axis", (float*)(&position.x), -100.f, 100.f);
            ImGui::SliderFloat("y-axis", (float*)(&position.y), -100.f, 100.f);
            ImGui::SliderFloat("z-axis", (float*)(&position.z), -100.f, 100.f);

            ImGui::Text("Set color");
            ImGui::ColorEdit3("Color ligth", (float*)(&color));
        ImGui::End();
    }

    void Widget::WindowAddObject(core::TypeObject& type, glm::vec3& position, glm::vec3& size, glm::vec3& color, bool& state_button) const {
        static int item_selected_idx = 0;

        ImGui::Begin("Object");
            ImGui::Text("Settings object");
            ImGui::ColorEdit3("Color", (float*)(&color));

            ImGui::Text("Position object:");
            ImGui::SliderFloat("x-axis", (float*)(&position.x), -100.f, 100.f);
            ImGui::SliderFloat("y-axis", (float*)(&position.y), -100.f, 100.f);
            ImGui::SliderFloat("z-axis", (float*)(&position.z), -100.f, 100.f);

            ImGui::Text("Size object:");
            ImGui::SliderFloat("x", (float*)(&size.x), 0.f, 100.f);
            ImGui::SliderFloat("y", (float*)(&size.y), 0.f, 100.f);
            ImGui::SliderFloat("z", (float*)(&size.z), 0.f, 100.f);

            ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * 4);
            if (ImGui::BeginListBox("Type object:", listbox_size)) {
                const bool is_selected = (item_selected_idx == 0);
                
                if (ImGui::Selectable("cube", is_selected)) {
                    item_selected_idx = 1;
                }

                if (ImGui::Selectable("pyramid", is_selected)) {
                    item_selected_idx = 2;
                }

                if (ImGui::Selectable("light", is_selected)) {
                    item_selected_idx = 3;
                }
                
                if (item_selected_idx > 0) {
                    ImGui::SetItemDefaultFocus();
                    if (item_selected_idx == 1) {
                        type = core::TypeObject::kCube;
                    }
                    else if (item_selected_idx == 2) {
                        type = core::TypeObject::kPyramid;
                    }
                    else if (item_selected_idx == 3) {
                        type = core::TypeObject::kLight;
                    }
                }

                ImGui::EndListBox();
            }

            state_button = ImGui::Button("Add Object");
        ImGui::End();
    }   

    int Widget::WindowListObjects(const std::vector<core::SceneObject>& objects) const {
        static int item_selected_idx = 0;
        int selected_item_id = 0;
        std::string type_object;

        if (ImGui::Begin("Objects")) {
            // ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * objects.size());
            if (ImGui::BeginListBox("Objects")) {
                for (int n = 0; n < objects.size(); ++n) {
                    std::string id("id " + std::to_string(objects[n].object.GetId()));
                    const bool is_selected = (item_selected_idx == n);

                    if (ImGui::Selectable(id.c_str(), is_selected)) {
                        item_selected_idx = n;
                    }

                    if (is_selected) {
                        // std::cout << id << '\n';
                        ImGui::SetItemDefaultFocus();
                        selected_item_id = objects[n].object.GetId();
                    }
                }
            ImGui::EndListBox(); 
            }
        }
        // TODO: Overhead costs due to convert int to string and to const char
        std::string str("Select object : " + std::to_string(selected_item_id));
        ImGui::Text(str.c_str());
        ImGui::End();
        return selected_item_id;
    }

    void Widget::WindowLogs(Logs logs) const {
        ImGui::Begin("Logs");
            ImGui::Text("fps: %i", logs.fps);
            ImGui::Text("delay: %.2f ms", logs.delay);
        ImGui::End();
    }

} // namespace avion::gui