#include "AvionEngineEditor/panels/inspector_panel.hpp"

namespace avion::editor::panel
{

  InspectorPanel::InspectorPanel(InspectorPanel::EditorContext editor_ctx, SelectionContext& select_ctx)
  : m_editor_ctx(editor_ctx)
  , m_select_ctx(select_ctx)
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
      if (m_select_ctx.model)
      { 
        RenderTabModel();
      }
      else if (m_select_ctx.light)
      {
        RenderTabLight();
      }
      else if (m_select_ctx.primitive)
      {
        RenderTabPrimitive();
      }
    }
  }

  void InspectorPanel::RenderTabModel() const noexcept 
  {
    ImGui::Text("Model");

    auto* model_obj = m_editor_ctx.engine.GetScene().GetModel(m_select_ctx.model.filename);
    if (model_obj)
    {
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

    auto* light_obj = m_editor_ctx.engine.GetScene().GetLight(m_select_ctx.light.id);
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
      
      switch (light_obj->type_light)
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

    auto* primitive_obj = m_editor_ctx.engine.GetScene().GetObject(m_select_ctx.primitive.id);
    if (primitive_obj)
    {
      auto& transform = primitive_obj->GetTransform();
      auto& color = primitive_obj->GetColor();
      auto& material = primitive_obj->GetMaterial();
      
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
    }
  }

} // namespace avion::editor::panel