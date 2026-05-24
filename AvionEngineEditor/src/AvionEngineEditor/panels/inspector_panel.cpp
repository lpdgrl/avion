#include "AvionEngineEditor/panels/inspector_panel.hpp"

namespace avion::editor::panel
{

  InspectorPanel::InspectorPanel(InspectorPanel::EditorContext& editor_ctx)
  : m_editor_ctx(editor_ctx)
  {

  }

  void InspectorPanel::Render() 
  {
    ImVec2 pos(1320, 0);
    ImVec2 size(600, 1080);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Inspector", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }

    RenderTabSelectObject();
  }

  void InspectorPanel::RenderTabSelectObject() const noexcept
  {
    TabBar tab("inspector_tab");
    TabItem item("Object");
    if (item.IsOpen())
    {
      if (m_editor_ctx.selection_ctx.model)
      { 
        RenderTabModel();
      }
      else if (m_editor_ctx.selection_ctx.light)
      {
        RenderTabLight();
      }
      else if (m_editor_ctx.selection_ctx.primitive)
      {
        RenderTabPrimitive();
      }
    }
  }

  void InspectorPanel::RenderTabModel() const noexcept 
  {
    auto& select_ctx = m_editor_ctx.selection_ctx.model;
    if (auto* model_obj = m_editor_ctx.engine.GetScene().GetModel(select_ctx.id, select_ctx.filename); model_obj != nullptr)
    {
      std::string msg("Model:");
      msg.append(m_editor_ctx.selection_ctx.model.filename);
      msg.append(" id:");
      msg.append(std::to_string(model_obj->id));
      ImGui::Text(msg.c_str());

      auto& transform = model_obj->model.GetTransform();
      
      ImGui::Text("Position");
      ui::utils::SliderFloat3V("position", transform.position, -10.f, 10.f);
      ui::utils::InputFloat3V("inp_position", transform.position, 0.25f, 0.5f);

      ImGui::Text("Size");
      ui::utils::SliderFloat3V("size", transform.size, 0.f, 10.f);
      ui::utils::InputFloat3V("inp_size", transform.size, 0.25f, 0.5f);

      ImGui::Text("Rotate");
      ImGui::SliderFloat3("##rotate", &transform.rotation.x, -180.f, 180.f);
    }
  }

  void InspectorPanel::RenderTabLight() const noexcept 
  {
    using namespace core;

    ImGui::Text("Light");

    auto* light_obj = m_editor_ctx.engine.GetScene().GetLight(m_editor_ctx.selection_ctx.light.id);
    if (light_obj)
    {
      auto& position = light_obj->light->GetGeometry();
      ImGui::Text("Position");
      ui::utils::SliderFloat3V("position", position, -30.f, 30.f);
      ui::utils::InputFloat3V("inp_position", position, 0.25f, 0.5f);

      auto diffuse  = light_obj->light->GetDiffuse();
      auto ambient  = light_obj->light->GetAmbient();
      auto specular = light_obj->light->GetSpecular();
      ImGui::Text("Ambient");
      ImGui::ColorEdit3("##ambient_point_light", &ambient.x);
      ImGui::Text("Diffuse");
      ImGui::ColorEdit3("##diffuse_point_light", &diffuse.x);
      ImGui::Text("Specular");
      ImGui::ColorEdit3("##specular_point_light", &specular.x);
      
      switch (light_obj->type)
      {
        case LightType::kPointLight:
        {
          auto *ptr = static_cast<PointLight*>(light_obj->light.get());
          float constant = ptr->GetConstant();
          float linear = ptr->GetLinear();
          float quadratic = ptr->GetQuadratic();

          ImGui::Text("Constant value");
          ImGui::InputFloat("##constant", &constant, 0.25f, 0.5f);
          ImGui::Text("Linear value");
          ImGui::InputFloat("##linear", &linear, 0.25f, 0.5f);
          ImGui::Text("Quadratic value");
          ImGui::InputFloat("##quadratic", &quadratic, 0.25f, 0.5f);

          ptr->SetConstant(constant);
          ptr->SetLinear(linear);
          ptr->SetQuadratic(quadratic);
          break;
        }

        case LightType::kDirLight:
        {
          break;
        }

        case LightType::kSpotLight:
        {
          auto *ptr = static_cast<SpotLight*>(light_obj->light.get());
          float constant = ptr->GetConstant();
          float linear = ptr->GetLinear();
          float quadratic = ptr->GetQuadratic();
          float cutoff = ptr->GetCutOff();
          float outer_cutoff = ptr->GetOuterCutOff();

          ImGui::Text("Constant value");
          ImGui::InputFloat("##constant", &constant, 0.25f, 0.5f);
          ImGui::Text("Linear value");
          ImGui::InputFloat("##linear", &linear, 0.25f, 0.5f);
          ImGui::Text("Quadratic value");
          ImGui::InputFloat("##quadratic", &quadratic, 0.25f, 0.5f);
          ImGui::Text("Cutoff value");
          ImGui::InputFloat("##cutoff", &cutoff, 0.25f, 0.5f);
          ImGui::Text("Outer cutoff value");
          ImGui::InputFloat("##outer_cutoff", &outer_cutoff, 0.25f, 0.5f);

          ptr->SetConstant(constant);
          ptr->SetLinear(linear);
          ptr->SetQuadratic(quadratic);
          ptr->SetCutOff(cutoff);
          ptr->SetOuterCutOff(outer_cutoff);
          break;
        }
      }

      light_obj->light->SetAmbient(ambient);
      light_obj->light->SetDiffuse(diffuse);
      light_obj->light->SetSpecular(specular);
    }
  }

  void InspectorPanel::RenderTabPrimitive() const noexcept 
  {
    ImGui::Text("Primitive");

    auto* primitive_obj = m_editor_ctx.engine.GetScene().GetObject(m_editor_ctx.selection_ctx.primitive.id);
    if (primitive_obj)
    {
      auto& transform = primitive_obj->object.GetTransform();
      auto& color = primitive_obj->object.GetColor();
      auto& material = primitive_obj->object.GetMaterial();

      ImGui::Text("Position");
      ui::utils::SliderFloat3V("position", transform.position, -10.f, 10.f);
      ui::utils::InputFloat3V("inp_position", transform.position, 0.25f, 0.5f);

      ImGui::Text("Size");
      ui::utils::SliderFloat3V("size", transform.size, 0.f, 10.f);
      ui::utils::InputFloat3V("inp_size", transform.size, 0.25f, 0.5f);

      ImGui::Text("Color");
      ImGui::ColorEdit3("##color", &color.color.x);

      ImGui::Text("Ambient");
      ImGui::ColorEdit3("##ambient", &material.ambient.x);

      ImGui::Text("Diffuse");
      ImGui::ColorEdit3("##diffuse", &material.diffuse.x);

      ImGui::Text("Specular");
      ImGui::ColorEdit3("##specular", &material.specular.x);

      ImGui::Text("Shininess");
      ImGui::DragFloat("##shininess", &material.shininess);

      ImGui::Text("Rotate");
      ImGui::SliderFloat3("##rotate", &transform.rotation.x, -180.f, 180.f);

      auto& textures = m_editor_ctx.engine.GetResourceManager().GetListTexture();

      ImGui::Text("Diffuse texture");
      static size_t index_item_diffuse_selected = 0;
      index_item_diffuse_selected = DrawComboTexture("diffuse", index_item_diffuse_selected, textures, material);

      ImGui::Text("Specular texture");
      static size_t index_item_specular_selected = 0;
      index_item_specular_selected = DrawComboTexture("specular", index_item_specular_selected, textures, material);
    }
  }

  size_t InspectorPanel::DrawComboTexture(const char* label, 
    size_t index_selected_texture,
    const core::resman::ResourceManager::ListTexture& textures,
    core::Material& material) const noexcept
  {
    const char* combo_preview_value = textures[index_selected_texture].c_str();

    std::string combo_name("##combo_");
    combo_name.append(label);

    ComboItem combo_specular(combo_name.c_str(), combo_preview_value);
    if (combo_specular.IsOpen())
    {
      for (int n = 0; auto& name : textures)
      {
        const bool is_selected = (index_selected_texture == n);
        if (ImGui::Selectable(name.c_str(), is_selected))
        {
          ImGui::SetItemDefaultFocus();
          index_selected_texture = n;
        }
        ++n;
      }
    }

    std::string button_name("Apply ");
    button_name.append(label);

    if (ImGui::Button(button_name.c_str()))
    {
      auto* texture = m_editor_ctx.engine.GetResourceManager().GetResource<core::Texture>(textures[index_selected_texture]);
      if (texture != nullptr)
      {
        material.is_texture = true;
        if (label == "diffuse")
        {
          material.texture_diffuse = texture->GetId();
        }
        else if (label == "specular")
        {
          material.texture_specular = texture->GetId();
        }
      }
    }

    return index_selected_texture;
  }

} // namespace avion::editor::panel