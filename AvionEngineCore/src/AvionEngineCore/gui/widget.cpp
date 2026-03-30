#include "AvionEngineCore/gui/widget.hpp"
#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/object.hpp"

namespace avion::gui {
    
    Widget::Widget(GLFWwindow* window, gfx::ResTextures& res)
        : window_(window)
        , io_(ImGui::GetIO()) 
        , res_(res)
    {
        core::InitPrefabMaterials();
    }

    Widget::~Widget() 
    {
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
        static int item_selected_idx     = 0;
        static int item_selected_idx_mat = 0;
        static int item_selected_idx_tex = 0;

        static bool checked_list_materials = false;
        static bool checked_list_textures  = false;
        static bool changed_size = false;

        static std::vector<std::string> type_materials{
          "None",
          "Emerald", 
          "Gold", 
          "Jade",
          "Obsidian",
          "Pearl",
          "Ruby",
          "Turquoise",
          "Brass",
          "Bronze",
          "Chrome",
          "Copper",
          "Silver",
          "Black plastic",
          "Cyan plastic",
          "Green plastic",
          "Red plastic",
          "White plastic",
          "Yellow plastic",
          "Black rubber",
          "Cyan rubber",
          "Green rubber",
          "Red rubber",
          "White rubber",
          "Yellow rubber"
        };

        static std::vector<std::string> types_objects{"cube", "pyramid"}; 
        static std::vector<std::string> textures{"None", "container2.png"};

        static WidgetObjectParams obj;
        ImGui::Begin("Object");
            ImGui::Text("Settings object");
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&obj.params.color));

            ImGui::Text("Position object:");
            ImGui::InputFloat3("Position", reinterpret_cast<float*>(&obj.params.position));

            ImGui::Text("Size object:");
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
                            obj.type_obj = core::ObjectType::kCube;
                        }
                        else if (item_selected_idx == 1) {
                            obj.type_obj = core::ObjectType::kPyramid;
                        }
                    } 
                }
                ImGui::EndListBox();
            }
    
            // List of materials 
            bool checked_lm = ImGui::Checkbox("Enable material", &checked_list_materials);

            if (checked_list_materials) {
              ImGui::Text("Prefab materials:");
              ImVec2 listbox_mat_sz(200.f, ImGui::GetTextLineHeightWithSpacing() * 4);
              if (ImGui::BeginListBox("##empty", listbox_mat_sz)) {
                  for (std::size_t i = 0; i < type_materials.size(); ++i) {
                      const bool is_selected = (item_selected_idx_mat == i);

                      if (ImGui::Selectable(type_materials[i].c_str(), is_selected)) {
                          item_selected_idx_mat = i;
                      }

                      if (is_selected) {
                          ImGui::SetItemDefaultFocus();
                          if (obj.type_obj == core::ObjectType::kCube || obj.type_obj == core::ObjectType::kPyramid) {
                              obj.type_mat = core::GetEnumValuePrefabMatByIndex(item_selected_idx_mat);
                              obj.params.material = core::GetPrefabMaterialByIndex(item_selected_idx_mat);
                          }
                      }
                  }
                  ImGui::EndListBox();
              }   
            }


            // List of textures 
            bool checked_lt = ImGui::Checkbox("Enable textures", &checked_list_textures);

            if (checked_list_textures) {
              ImVec2 listbox_tex_sz(200.f, ImGui::GetTextLineHeightWithSpacing() * 4);
              if (ImGui::BeginListBox("##empty:", listbox_tex_sz)) {
                  for (std::size_t i = 0; i < textures.size(); ++i) {
                      const bool is_selected = (item_selected_idx_tex == i);

                      if (ImGui::Selectable(textures[i].c_str(), is_selected)) {
                          item_selected_idx_tex = i;
                      }
                      
                      if (is_selected) {
                          ImGui::SetItemDefaultFocus();
                          if (obj.type_obj == core::ObjectType::kCube) {
                              if (item_selected_idx_tex == 0) {
                                obj.params.material.texture = nullptr;
                                obj.params.material.texture_specular = nullptr;
                                obj.params.material.texture_emission = nullptr;
                              } else if (item_selected_idx_tex == 1) {
                                  obj.params.material.texture = res_.at(textures[item_selected_idx_tex]);
                                  // obj.params.material.texture = res_.at("RubicDiffuse.jpg");
                                  obj.params.material.texture_specular = res_.at("container2_specular.png");
                                  // obj.params.material.texture_specular = res_.at("lighting_maps_specular_color.png");
                                  // obj.params.material.texture_emission = res_.at("matrix.jpg");
                                  // obj.params.material.texture_emission = res_.at("RubicEmissive.jpg");
                              }
                          }
                      }
                  }
                  ImGui::EndListBox();
              } 
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
            arr_changed_material[9] = ImGui::SliderFloat("mat shininess component slider", &material.shininess, 0.f, 1000.f);

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

        auto& light = params.light;
        auto& position = light->GetGeometry();
        auto& size  = params.size;
        auto& color = params.color;
        
        auto ambient = light->GetAmbient();
        auto diffuse = light->GetDiffuse();
        auto specular = light->GetSpecular();

        ImGui::Begin("Light properties");
            arr_changed_prop_light[0] = ImGui::SliderFloat("light pos x", &position.x, -20.f, 20.f);
            arr_changed_prop_light[1] = ImGui::SliderFloat("light pos y", &position.y, -20.f, 20.f);
            arr_changed_prop_light[2] = ImGui::SliderFloat("light pos z", &position.z, -20.f, 20.f);

            arr_changed_prop_light[3] = ImGui::InputFloat3("light size", reinterpret_cast<float*>(&size));
            arr_changed_prop_light[4] = ImGui::SliderFloat3("light size slider", reinterpret_cast<float*>(&size), 0.f, 20.f);
            
            arr_changed_prop_light[5] = ImGui::ColorEdit3("light color", reinterpret_cast<float*>(&color)); 

            arr_changed_prop_light[6] = ImGui::ColorEdit3("light ambient component", reinterpret_cast<float*>(&ambient));
            arr_changed_prop_light[7] = ImGui::ColorEdit3("light diffuse component", reinterpret_cast<float*>(&diffuse));
            arr_changed_prop_light[8] = ImGui::ColorEdit3("light specular component", reinterpret_cast<float*>(&specular));

        ImGui::End();

        if (arr_changed_prop_light[6]) {
            light->SetAmbient(ambient);
        } 

        if (arr_changed_prop_light[7]) {
            light->SetDiffuse(diffuse);
        }
        
        if (arr_changed_prop_light[8]) {
            light->SetSpecular(specular);
        }

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
        std::string t_light;

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
                        auto type_light = lights[i].type_light;

                        if (type_light == core::LightType::kDirLight) {
                          t_light.append("DirLight");
                        } else if (type_light == core::LightType::kPointLight) {
                          t_light.append("PointLight");
                        } else if (type_light == core::LightType::kSimpleLight) {
                          t_light.append("SimpleLight");
                        }
                    }
                }
                ImGui::EndListBox(); 
            }
        }
        // TODO: Overhead costs due to convert int to string and to const char
        std::string str("Selected was source light:\n\tid = " + std::to_string(selected_item_id) + t_light);
        
        ImGui::Text(str.c_str());
        ImGui::End();

        return selected_item_id;
    }

    std::optional<core::LightType> Widget::w_LightAdd() const 
    {
        static int item_selected_idx = 0;
        static std::vector<std::string> type_source_lights{"simple light", "direction light", "point light"}; 
        core::LightType type;
        
        ImGui::Begin("Add source light");
            ImVec2 listbox_size(200.0f, ImGui::GetTextLineHeightWithSpacing() * 4);
            if (ImGui::BeginListBox("Type:", listbox_size)) {
                for (int i = 0; i < type_source_lights.size(); ++i) {
                    const bool is_selected = (item_selected_idx == i);
                    
                    if (ImGui::Selectable(type_source_lights[i].c_str(), is_selected)) {
                        item_selected_idx = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        if (item_selected_idx == 0) {
                          type = core::LightType::kSimpleLight;     
                        } else if (item_selected_idx == 1) {
                          type = core::LightType::kDirLight;     
                        } else if (item_selected_idx == 2) {
                          type = core::LightType::kPointLight;
                        }
                    }
                } 
            }
            ImGui::EndListBox(); 

            if(ImGui::Button("Add light")) {
                ImGui::End();
                return type;
            }

        ImGui::End();
        return std::nullopt;
    }                   

} // namespace avion::gui
