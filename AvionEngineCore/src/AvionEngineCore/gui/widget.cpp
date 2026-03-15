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


    std::optional<WidgetObjectParams> Widget::WindowAddObject() const {
        static int item_selected_idx = 0;
        static int item_selected_idx_mat = 0;
        static bool changed_size = false;
        static std::vector<std::string> types_objects{"cube", "pyramid", "light"};
        static std::vector<std::string> type_materials{"Emerald", "Gold", "Black plastic"};
        
        static WidgetObjectParams obj;
        ImGui::Begin("Object");
            ImGui::Text("Settings object");
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&obj.params.color));

            ImGui::Text("Position object:");
            // ImGui::SliderFloat("x-axis", reinterpret_cast<float*>(&obj.params.position.x), -20.f, 20.f);
            // ImGui::SliderFloat("y-axis", reinterpret_cast<float*>(&obj.params.position.y), -20.f, 20.f);
            // ImGui::SliderFloat("z-axis", reinterpret_cast<float*>(&obj.params.position.z), -20.f, 20.f);
            ImGui::InputFloat3("Position", reinterpret_cast<float*>(&obj.params.position));

            ImGui::Text("Size object:");
            // ImGui::SliderFloat("x", reinterpret_cast<float*>(&obj.params.size.x), 0.f, 20.f);
            // ImGui::SliderFloat("y", reinterpret_cast<float*>(&obj.params.size.y), 0.f, 20.f);
            // ImGui::SliderFloat("z", reinterpret_cast<float*>(&obj.params.size.z), 0.f, 20.f);
            
            changed_size = ImGui::InputFloat3("Size", reinterpret_cast<float*>(&obj.params.size));

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
                            if (!changed_size) {
                                obj.params.size = glm::vec3(1.f, 1.f, 1.f);
                            } 
                            obj.type_obj = core::TypeObject::kCube;
                        }
                        else if (item_selected_idx == 1) {
                            obj.type_obj = core::TypeObject::kPyramid;
                        }
                        else if (item_selected_idx == 2) {
                            obj.type_obj = core::TypeObject::kLight;
                            obj.params.color = glm::vec3(1.f);
                            obj.params.size = glm::vec3(0.25f, 0.25f, 0.25f);
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
                        if (obj.type_obj == core::TypeObject::kCube || obj.type_obj == core::TypeObject::kPyramid) {
                            if (item_selected_idx_mat == 0) {
                                obj.type_mat = core::PrefabMaterial::kEmerald;
                                obj.params.material = core::prefab_Emerald;
                            } else if (item_selected_idx_mat == 1) {
                                obj.type_mat = core::PrefabMaterial::kGold;
                                obj.params.material = core::prefab_Gold;
                            } else if (item_selected_idx_mat == 2) {
                                obj.type_mat = core::PrefabMaterial::kBlackPlastic;
                                obj.params.material = core::prefab_BlackPlastic;
                            }
                        }
                    }
                }
                ImGui::EndListBox();
            } 

        if(ImGui::Button("Add Object")) {
            ImGui::End();
            return obj;
        }
            
        ImGui::End();
        return std::nullopt; 
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

    bool Widget::WindowMaterial(core::ObjectParams& object_params) const {
        static bool changed_material = false;
        static std::array<bool, 10> arr_changed_material; 

        auto& size = object_params.size;
        auto& position = object_params.position;
        auto& material = object_params.material;

        ImGui::Begin("Material properties");
            arr_changed_material[0] = ImGui::SliderFloat("obj pos x", &position.x, -20.f, 20.f);
            arr_changed_material[1] = ImGui::SliderFloat("obj pos y", &position.y, -20.f, 20.f);
            arr_changed_material[2] = ImGui::SliderFloat("obj pos z", &position.z, -20.f, 20.f);

            arr_changed_material[3] = ImGui::InputFloat3("obj size ", reinterpret_cast<float*>(&size));
            arr_changed_material[4] = ImGui::SliderFloat3("obj size slider", reinterpret_cast<float*>(&size), 0.f, 20.f);
            
            arr_changed_material[5] = ImGui::ColorEdit3("mat ambient component", reinterpret_cast<float*>(&material.ambient));
            arr_changed_material[6] = ImGui::ColorEdit3("mat diffuse component", reinterpret_cast<float*>(&material.diffuse));
            arr_changed_material[7] = ImGui::ColorEdit3("mat specular component", reinterpret_cast<float*>(&material.specular));
            arr_changed_material[8] = ImGui::InputFloat("mat shininess component", &material.shininess, 1.0f);
            arr_changed_material[9] = ImGui::SliderFloat("mat shininess component slider", &material.shininess, 0.f, 100.f);

        ImGui::End();

        changed_material = std::any_of(arr_changed_material.cbegin(), arr_changed_material.cend(), [](bool state_changed_value) {
                 return state_changed_value;}
        );
        std::fill(arr_changed_material.begin(), arr_changed_material.end(), false);

        return changed_material;
    }

    bool Widget::w_LightProperties(core::LightParams& params) const 
    {
        static bool changed_properties_light = false;
        static std::array<bool, 10> arr_changed_prop_light; 

        auto& light    = params.light;
        auto& position = light.position;
        auto& size  = params.size;
        auto& color = params.color;

        ImGui::Begin("Light properties");
            arr_changed_prop_light[0] = ImGui::SliderFloat("light pos x", &position.x, -20.f, 20.f);
            arr_changed_prop_light[1] = ImGui::SliderFloat("light pos y", &position.y, -20.f, 20.f);
            arr_changed_prop_light[2] = ImGui::SliderFloat("light pos z", &position.z, -20.f, 20.f);

            arr_changed_prop_light[3] = ImGui::InputFloat3("light size", reinterpret_cast<float*>(&size));
            arr_changed_prop_light[4] = ImGui::SliderFloat3("light size slider", reinterpret_cast<float*>(&size), 0.f, 20.f);
            
            arr_changed_prop_light[5] = ImGui::ColorEdit3("light color", reinterpret_cast<float*>(&color)); 

            arr_changed_prop_light[6] = ImGui::ColorEdit3("light ambient component", reinterpret_cast<float*>(&light.ambient));
            arr_changed_prop_light[7] = ImGui::ColorEdit3("light diffuse component", reinterpret_cast<float*>(&light.diffuse));
            arr_changed_prop_light[8] = ImGui::ColorEdit3("light specular component", reinterpret_cast<float*>(&light.specular));

        ImGui::End();

        changed_properties_light = std::any_of(arr_changed_prop_light.cbegin(), arr_changed_prop_light.cend(), [](bool state_changed_value) {
                 return state_changed_value;}
        );
        std::fill(arr_changed_prop_light.begin(), arr_changed_prop_light.end(), false);

        return changed_properties_light;
    }

    int Widget::w_ListLights(const std::vector<core::SceneLight>& lights) const 
    {
        static int item_selected_idx = 0;
        int selected_item_id = 0;

        if (ImGui::Begin("Source lights")) {
            if (ImGui::BeginListBox("Lights")) {
                for (int i = 0; i < lights.size(); ++i) {
                    std::string id(
                        " id:" 
                        + std::to_string(lights[i].id
                    ));
                    const bool is_selected = (item_selected_idx == i);

                    if (ImGui::Selectable(id.c_str(), is_selected)) {
                        item_selected_idx = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        selected_item_id = lights[i].id; 
                    }
                }
                ImGui::EndListBox(); 
            }
        }
        // TODO: Overhead costs due to convert int to string and to const char
        std::string str("Selected was source light:\n\tid = " + std::to_string(selected_item_id));
        ImGui::Text(str.c_str());
        ImGui::End();
        return selected_item_id;
    }
} // namespace avion::gui
