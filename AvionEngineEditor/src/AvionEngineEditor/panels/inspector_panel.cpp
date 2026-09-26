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

  void InspectorPanel::RenderTabSelectItem() const noexcept
  {
    ImGui::Text("Item:");

    auto id = m_editor_ctx.selection_ctx.item.id;
    auto&& opt = m_editor_ctx.engine.GetScene().GetItem(id);

    if (opt.has_value())
    {
      auto&& item = opt.value();
      auto& transform = item.ptr_model->GetTransform();
      auto& color = item.ptr_model->GetMaterial().color;
      auto& material = item.ptr_model->GetMaterial();

      ImGui::Text("Position");
      ui::utils::SliderFloat3V("position", transform.position, -10.f, 10.f);
      ui::utils::InputFloat3V("inp_position", transform.position, 0.25f, 0.5f);

      ImGui::Text("Size");
      ui::utils::SliderFloat3V("size", transform.size, 0.f, 10.f);
      ui::utils::InputFloat3V("inp_size", transform.size, 0.25f, 0.5f);

      ImGui::Text("Color");
      ImGui::ColorEdit3("##color", &color.x);

      if (item.item_type != core::ItemType::kSourceLight)
      {
        ImGui::Text("Shininess");
        ImGui::DragFloat("##shininess", &material.shininess);
      }
      ImGui::Text("Rotate");
      ImGui::SliderFloat3("##rotate", &transform.rotation.x, -180.f, 180.f);

      if (item.item_type == core::ItemType::kSourceLight)
      {
        RenderTabLight();
        return;
      }

      auto& textures = m_editor_ctx.engine.GetResourceManager().GetListTexture();

      ImGui::Text("Diffuse texture");
      static size_t index_item_diffuse_selected = 0;
      index_item_diffuse_selected = DrawComboTexture("diffuse", index_item_diffuse_selected, textures, material);

      ImGui::Text("Specular texture");
      static size_t index_item_specular_selected = 0;
      index_item_specular_selected = DrawComboTexture("specular", index_item_specular_selected, textures, material);

      DeleteFromScene(id);
    }
  
  }

  void InspectorPanel::RenderTabSelectObject() const noexcept
  {
    TabBar tab("inspector_tab");
    TabItem item("Object");
    if (item.IsOpen())
    {
      if (m_editor_ctx.selection_ctx.item)
      {
        RenderTabSelectItem();
      }
    }
  }

  void InspectorPanel::RenderTabLight() const noexcept
  {
    using namespace core;

    ImGui::Text("Light");
    auto id = m_editor_ctx.selection_ctx.item.id;
    auto&& opt = m_editor_ctx.engine.GetScene().GetItem(id);
    if (opt.has_value())
    {
      auto& item = static_cast<core::LightItem&>(opt.value());
      // auto& position = item.light->GetGeometry();
      // ImGui::Text("Position");
      // ui::utils::SliderFloat3V("position", position, -30.f, 30.f);
      // ui::utils::InputFloat3V("inp_position", position, 0.25f, 0.5f);

      auto diffuse  = item.light->GetDiffuse();
      auto ambient  = item.light->GetAmbient();
      auto specular = item.light->GetSpecular();
      ImGui::Text("Ambient");
      ImGui::ColorEdit3("##ambient_point_light", &ambient.x);
      ImGui::Text("Diffuse");
      ImGui::ColorEdit3("##diffuse_point_light", &diffuse.x);
      ImGui::Text("Specular");
      ImGui::ColorEdit3("##specular_point_light", &specular.x);

      switch (item.light_type)
      {
        case LightType::kPointLight:
        {
          auto *ptr = static_cast<PointLight*>(item.light.get());
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
          auto *ptr = static_cast<SpotLight*>(item.light.get());
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

      item.light->SetAmbient(ambient);
      item.light->SetDiffuse(diffuse);
      item.light->SetSpecular(specular);
    }

    DeleteFromScene(id);
  }

  size_t InspectorPanel::DrawComboTexture
  (
    const char* label,
    size_t index_selected_texture,
    const core::resman::ResourceManager::ListTexture& textures,
    gfx::Model::Material& material
  ) const noexcept
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

    if (!ImGui::Button(button_name.c_str()))
    {
      return index_selected_texture;
    }

    auto* texture = m_editor_ctx.engine.GetResourceManager().GetResource<core::Texture>(textures[index_selected_texture]);
    if (texture == nullptr)
    {
      assert(texture);
      AV_LOG_ERROR("InspectorPanel::DrawComboTexture: texture is nullptr!");
      return 0;
    }

    auto& texture_manager = m_editor_ctx.engine.GetTextureManager();
    material.type = core::material::MaterialType::kTexture;
    if (label == "diffuse")
    {
      if (texture_manager.Contains(textures[index_selected_texture]))
      {
        auto texture_item = texture_manager.Get(textures[index_selected_texture]).value();
        // TODO: Pontetial leak texture ptr
        material.material_diffuse_override = {core::texturemanager::detail::TextureType::kDiffuse, texture_item.id};
        return index_selected_texture;
      }

      auto result = texture_manager.Load(textures[index_selected_texture]).value();
      material.material_diffuse_override = {core::texturemanager::detail::TextureType::kDiffuse, result.id};
    }
    else if (label == "specular")
    {
      if (texture_manager.Contains(textures[index_selected_texture]))
      {
        auto texture_item = texture_manager.Get(textures[index_selected_texture]).value();
        material.material_specular_override = {core::texturemanager::detail::TextureType::kSpecular, texture_item.id};
        return index_selected_texture;
      }

      auto result = texture_manager.Load(textures[index_selected_texture]).value();
      material.material_specular_override = {core::texturemanager::detail::TextureType::kSpecular, result.id};
    }
    return index_selected_texture;
  }

  auto InspectorPanel::DeleteFromScene(std::uint32_t id) const noexcept -> void
  {
    if (ImGui::Button("Delete item from scene"))
    {
      if (auto result = m_editor_ctx.engine.GetScene().DeleteItem(id); !result)
      {
        AV_LOG_ERROR(std::format("The item with id = {} isn't deleted from scene!", id));
      }
      m_editor_ctx.selection_ctx.item.id = 0;
      m_editor_ctx.selection_ctx.item.is_select = false;
    }
  }

} // namespace avion::editor::paneldww
