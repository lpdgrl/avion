#include "AvionEngineEditor/panels/asset_panel.hpp"

namespace avion::editor::panel
{

  AssetPanel::AssetPanel(AssetPanel::EditorContext context)
  : m_context(context)
  {

  }

  void AssetPanel::Render() 
  {

    ImVec2 pos(0, 700);
    ImVec2 size(1320, 380);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Assets", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }
  }

} // namespace avion::editor::panel