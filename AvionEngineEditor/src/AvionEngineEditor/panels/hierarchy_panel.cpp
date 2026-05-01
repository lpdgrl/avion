#include "AvionEngineEditor/panels/hierarchy_panel.hpp"

namespace avion::editor::panel
{

  HierarchyPanel::HierarchyPanel(HierarchyPanel::EditorContext context)
  : m_context(context)
  {

  }

  void HierarchyPanel::Render() 
  {

    ImVec2 pos(0, 200);
    ImVec2 size(300, 500);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Hierarchy", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }
  }

} // namespace avion::editor::panel