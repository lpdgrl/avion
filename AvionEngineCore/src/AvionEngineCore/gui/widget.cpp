#include "AvionEngineCore/gui/widget.hpp"
#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/object.hpp"

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

    void Widget::WindowAddObject(WidgetObjectParams& obj) const {
        static int item_selected_idx = 0;
        static int item_selected_idx_mat = 0;
        static std::vector<std::string> types_objects{"cube", "pyramid", "light"};
        static std::vector<std::string> type_materials{"Emerald", "Gold", "Black plastic"};

        ImGui::Begin("Object");
            ImGui::Text("Settings object");
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&obj.params.color));


            ImGui::Text("Position object:");
            ImGui::SliderFloat("x-axis", reinterpret_cast<float*>(&obj.params.position.x), -20.f, 20.f);
            ImGui::SliderFloat("y-axis", reinterpret_cast<float*>(&obj.params.position.y), -20.f, 20.f);
            ImGui::SliderFloat("z-axis", reinterpret_cast<float*>(&obj.params.position.z), -20.f, 20.f);

            ImGui::Text("Size object:");
            ImGui::SliderFloat("x", reinterpret_cast<float*>(&obj.params.size.x), 0.f, 20.f);
            ImGui::SliderFloat("y", reinterpret_cast<float*>(&obj.params.size.y), 0.f, 20.f);
            ImGui::SliderFloat("z", reinterpret_cast<float*>(&obj.params.size.z), 0.f, 20.f);

            ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * 4);
            if (ImGui::BeginListBox("Type:", listbox_size)) {
                for (int i = 0; i < types_objects.size(); ++i) {
                    const bool is_selected = (item_selected_idx == i);
                    
                    if (ImGui::Selectable(types_objects[i].c_str(), is_selected)) {
                        item_selected_idx = i;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        if (item_selected_idx == 0) {
                            obj.type_obj = core::TypeObject::kCube;
                        }
                        else if (item_selected_idx == 1) {
                            obj.type_obj = core::TypeObject::kPyramid;
                        }
                        else if (item_selected_idx == 2) {
                            obj.type_obj = core::TypeObject::kLight;
                        }
                    } 
                }
                ImGui::EndListBox();
            }
    
            // List of materials 
            ImVec2 listbox_mat_sz(200.f, ImGui::GetTextLineHeightWithSpacing() * 4);
            if (ImGui::BeginListBox("Material:", listbox_mat_sz)) {
                for (std::size_t i = 0; i < type_materials.size(); ++i) {
                    const bool is_selected = (item_selected_idx_mat == i);

                    if (ImGui::Selectable(type_materials[i].c_str(), is_selected)) {
                        item_selected_idx_mat = i;
                    }
                    
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        if (item_selected_idx_mat == 0) {
                            obj.type_mat = core::TypeMaterial::kEmerald;
                        } else if (item_selected_idx_mat == 1) {
                            obj.type_mat = core::TypeMaterial::kGold;
                        } else if (item_selected_idx_mat == 2) {
                            obj.type_mat = core::TypeMaterial::kBlackPlastic;
                        }
                    }
                }
                ImGui::EndListBox();
            } 

            obj.state_button_addobj = ImGui::Button("Add Object");
        ImGui::End();
    }   

    int Widget::WindowListObjects(const std::vector<core::SceneObject>& objects) const {
        static int item_selected_idx = 0;
        int selected_item_id = 0;
        std::string type_object("None");

        if (ImGui::Begin("Objects")) {
            // ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * objects.size());
            if (ImGui::BeginListBox("Objects")) {
                for (int i = 0; i < objects.size(); ++i) {
                    std::string id(
                        core::TypeObjectToString(objects[i].type)
                        + " id:" 
                        + std::to_string(objects[i].object.GetId())
                    );
                    const bool is_selected = (item_selected_idx == i);

                    if (ImGui::Selectable(id.c_str(), is_selected)) {
                        item_selected_idx = i;
                    }

                    if (is_selected) {
                        // std::cout << id << '\n';
                        ImGui::SetItemDefaultFocus();
                        selected_item_id = objects[i].object.GetId();
                        type_object = core::TypeObjectToString(objects[i].type);
                    }
                }
            ImGui::EndListBox(); 
            }
        }
        // TODO: Overhead costs due to convert int to string and to const char
        std::string str("Selected was the object:\n\tid = " + std::to_string(selected_item_id) + "\n\ttype =  " + type_object);
        ImGui::Text(str.c_str());
        ImGui::End();
        return selected_item_id;
    }

    void Widget::WindowLogs(Logs logs) const {
        ImGui::Begin("Debug");
            ImGui::Text("Perfomance:");
            ImGui::Text("\tfps: %i", logs.fps);
            ImGui::Text("\tdelay: %.2f ms", logs.delay);
            ImGui::Text("Coordinates:");
            ImGui::Text("\tx_px: %.3f", logs.x_px);
            ImGui::Text("\ty_px: %.3f", logs.y_px);
            ImGui::Text("\tx_ndc: %.3f", logs.x_ndc);
            ImGui::Text("\ty_ndc: %.3f", logs.y_ndc);
        ImGui::End();
    }


    void Widget::WidgetShader(float& kSpecular, float& kAmbient, float& kShininess) const {
        ImGui::Begin("Shader");
            ImGui::SliderFloat("kAmbient", &kAmbient, 0.0, 1);
            ImGui::SliderFloat("kSpecular", &kSpecular, 0.0, 1);
            ImGui::InputFloat("kShininess", &kShininess);
        ImGui::End();
    }
    
} // namespace avion::gui
