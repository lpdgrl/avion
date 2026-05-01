#include "AvionEngineEditor/panels/profiler_panel.hpp"


namespace avion::editor::panel
{

  ProfilerPanel::ProfilerPanel(ProfilerPanel::ProfilerContext context)
  : m_context(context)
  {

  }

  void ProfilerPanel::Render() 
  {
    ImVec2 pos(0,0);
    ImVec2 size(300, 200);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Profiler", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }
    
   TabBar tab("prof_tab");
   ProfilerTab();
   CameraTab();
  }
  

  void ProfilerPanel::ProfilerTab() const noexcept
  {
    TabItem tab_item("Profiler");
    if (tab_item.IsOpen())
    {
      std::string text;
      text.append("FPS: ");
      text.append(std::to_string(m_context.profiler.frame_state.m_frames));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("Delay: ");
      text.append(std::to_string(m_context.profiler.frame_state.m_delay));
      text.append(" ms");
      ImGui::Text(text.c_str());
      text.clear();

      text.append("x_px cursor: ");
      text.append(std::to_string(m_context.profiler.cursor_pos.x_px_cursor));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("y_px cursor: ");
      text.append(std::to_string(m_context.profiler.cursor_pos.y_px_cursor));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("x_ndc cursor: ");
      text.append(std::to_string(m_context.profiler.cursor_pos.x_ndc_cursor));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("y_ndc cursor: ");
      text.append(std::to_string(m_context.profiler.cursor_pos.y_ndc_cursor));
      ImGui::Text(text.c_str());
      text.clear();
    }
  }

  void ProfilerPanel::CameraTab() const noexcept
  {
    TabItem tab_item("Camera");
    if (tab_item.IsOpen())
    {
      std::string text;
      ImGui::Text("Camera position:");
      text.append("x: ");
      text.append(std::to_string(m_context.profiler.camera_state.camera_position.x));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("y: ");
      text.append(std::to_string(m_context.profiler.camera_state.camera_position.y));
      ImGui::Text(text.c_str());
      text.clear();

      text.append("z: ");
      text.append(std::to_string(m_context.profiler.camera_state.camera_position.z));
      ImGui::Text(text.c_str());
      text.clear();
    }
  }

} // namespace avion::editor::panel