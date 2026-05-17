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
    static bool x_axis = false;
    static bool y_axis = false;
    static bool z_axis = false;

    ImGui::Text("Model");

    auto* model_obj = m_editor_ctx.engine.GetScene().GetModel(m_select_ctx.model.filename);
    if (model_obj)
    {
      auto& transform = model_obj->model.GetTransform();

      
      ImGui::Text("Position");
      ui::utils::SliderFloat3V("position", transform.position, -10.f, 10.f);
      ui::utils::InputFloat3V("inp_position", transform.position, 0.25f, 0.5f);

      ImGui::Text("Size");
      ui::utils::SliderFloat3V("size", transform.size, -10.f, 10.f);
      ui::utils::InputFloat3V("inp_size", transform.size, 0.25f, 0.5f);

      ImGui::Text("Rotate");

      if (!z_axis && !y_axis)
      {
        ImGui::Checkbox("x-axis", &x_axis);
      }
      if (!x_axis && !z_axis)
      {
        ImGui::Checkbox("y-axis", &y_axis);
      }
      if (!x_axis && !y_axis)
      {
        ImGui::Checkbox("z-axis", &z_axis);
      }

      ImGui::Text("Value rotate");
      ImGui::InputFloat("##rotate_inp", &transform.rotate, -180.f, 180.f);
      ImGui::SliderFloat("##rotate", &transform.rotate, -180.f, 180.f);

      if (x_axis)
      {
        transform.axis = gfx::AxisRotate::AXIS_X;
      }
      if (y_axis)
      {
        transform.axis = gfx::AxisRotate::AXIS_Y;
      }
      if (z_axis)
      {
        transform.axis = gfx::AxisRotate::AXIS_Z;
      }
      if (!x_axis && !y_axis && !z_axis)
      {
        transform.axis = gfx::AxisRotate::NONE;
      }
    }
  }

  void InspectorPanel::RenderTabLight() const noexcept 
  {
    
  }

  void InspectorPanel::RenderTabPrimitive() const noexcept 
  {
    
  }

} // namespace avion::editor::panel