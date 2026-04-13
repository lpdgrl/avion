#include "AvionEngineCore/gui/widget.hpp"
#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/object.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gui {
    
    Widget::Widget(GLFWwindow* window, core::resman::ResourceManager& resman)
        : window_(window)
        , io_(ImGui::GetIO()) 
        , m_resman(resman)
    {
        core::InitPrefabMaterials();
    }

    Widget::~Widget() 
    {
        CleanUp();
        AV_LOG_INFO("Widget::~Widget: Widget is destroyed");
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

    void Widget::TextureComboList(int& texture_id, std::string_view caption, const std::vector<std::string>& list_textures, int& item_selected) const
    {
      if (ImGui::BeginCombo(caption.data(), list_textures[item_selected].c_str()))
        {
          for (std::size_t n = 0; n < list_textures.size(); ++n)
          {
            const bool is_selected = (static_cast<std::size_t>(item_selected) == n);

            if (ImGui::Selectable(list_textures[n].c_str(), is_selected))
            {
              item_selected = n;
            }

            if (is_selected)
            { 
              // TODO: Each true condition get id is very bad. In future need to caching id
              std::string_view key = list_textures[item_selected];
              auto* res = m_resman.GetResource<core::Texture>(key);
              texture_id = res->GetId();
        
              std::string str;
              str += "Widget::TextureComboList: set texture ";
              str += caption.data();
              str += " id = " + std::to_string(res->GetId());
              AV_LOG_DEBUG(str);
              
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
    }

    std::optional<WidgetObjectParams> Widget::WindowAddObject() const {
      static int item_selected_idx          = 0;
      static int item_selected_idx_mat      = 0;
      static int item_selected_idx_tex      = 0;
      static int item_selected_idx_tex_spec = 0;
      static int item_selected_idx_tex_ems  = 0;

      static bool checked_list_materials   = false;
      static bool checked_texture          = false;
      static bool checked_texture_specular = false;
      static bool checked_texture_emission = false;
      static bool changed_size             = false;

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
      static std::vector<std::string> textures = m_resman.GetListTexture();

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

          if (checked_list_materials && !checked_texture) {
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
          [[maybe_unused]] bool checked_tx = ImGui::Checkbox("Enable texture", &checked_texture);
          if (checked_texture && !checked_list_materials) {
            auto& list_textures = m_resman.GetListTexture();
            obj.params.material.is_texture = true;
            TextureComboList(obj.params.material.texture_diffuse, "Diffuse", list_textures, item_selected_idx_tex);
            
            [[maybe_unused]] bool checked_txsp = ImGui::Checkbox("Enable texture specular", &checked_texture_specular);
            if (checked_texture && checked_texture_specular)
            {
              TextureComboList(obj.params.material.texture_specular, "Specular", list_textures, item_selected_idx_tex_spec);
            }

            [[maybe_unused]] bool checked_txem = ImGui::Checkbox("Enable texture emssion", &checked_texture_emission);
            if (checked_texture && checked_texture_emission)
            {
              TextureComboList(obj.params.material.texture_emission, "Emission", list_textures, item_selected_idx_tex_ems);
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
        
        changed_material = std::ranges::any_of(arr_changed_material, [](bool state_changed_value) {
                 return state_changed_value;});
        std::ranges::fill(arr_changed_material, false);

        return changed_material;
    }

    bool Widget::w_LightProperties(core::LightParams& params) const 
    {
      static bool changed_properties_light = false;
      static std::array<bool, 15> arr_changed_prop_light; 
      
      core::LightType light_type = core::LightType::kUnknownLight; 

      auto& light = params.light;
      auto& position = light->GetGeometry();
      auto& size  = params.size;
      auto& color = params.color;
      
      auto ambient = light->GetAmbient();
      auto diffuse = light->GetDiffuse();
      auto specular = light->GetSpecular();
      
      // Spotlight params
      float cut_off = 0.f;
      float outer_cut_off = 0.f;
      glm::vec3 direction;

      core::SpotLight* p_spotlight = nullptr;
      if (p_spotlight = dynamic_cast<core::SpotLight*>(light))
      {
        direction = p_spotlight->GetDirection();
        cut_off = p_spotlight->GetCutOff();
        outer_cut_off = p_spotlight->GetOuterCutOff();
      }

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
          
          if (p_spotlight != nullptr)
          {
            ImGui::Text("Spotlight");
            arr_changed_prop_light[9]  = ImGui::InputFloat("spotlight inner cutoff", &cut_off);
            arr_changed_prop_light[10] = ImGui::SliderFloat("spotlight inner cutoff slider", &cut_off, -50.f, 50.f);

            arr_changed_prop_light[11] = ImGui::InputFloat("spotlight outer cutoff", &outer_cut_off);
            arr_changed_prop_light[12] = ImGui::SliderFloat("spotlight outer cutoff slider", &outer_cut_off, -50.f, 50.f);

            arr_changed_prop_light[13] = ImGui::InputFloat3("spotlight direction", reinterpret_cast<float*>(&direction));
            arr_changed_prop_light[14] = ImGui::SliderFloat3("spotlight dir slider", reinterpret_cast<float*>(&direction), -50.f, 50.f);
          }
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

      if (p_spotlight != nullptr)
      {
        if (arr_changed_prop_light[9] || arr_changed_prop_light[10])
        {
          p_spotlight->SetCutOff(cut_off);
        }
        
        if (arr_changed_prop_light[11] || arr_changed_prop_light[12]) 
        {
          p_spotlight->SetOuterCutOff(outer_cut_off);
        }

        if (arr_changed_prop_light[13] || arr_changed_prop_light[14]) 
        {
          p_spotlight->SetDirection(direction);
        }
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
                        } else if (type_light == core::LightType::kSpotLight)
                        {
                          t_light.append("SpotLight");
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
        static std::vector<std::string> type_source_lights{ "direction light", "point light", "spotlight"}; 
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
                          type = core::LightType::kDirLight;     
                        } else if (item_selected_idx == 1) {
                          type = core::LightType::kPointLight;
                        } else if (item_selected_idx == 2) {
                          type = core::LightType::kSpotLight;
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
