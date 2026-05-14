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
    
    RenderAsset();
  }

  void AssetPanel::RenderAsset() const noexcept
  {
    const auto& model = m_context.engine.GetResourceManager().GetModelLoadedList();
    static int index_select = -1;

    for (size_t i = 0; i < model.size(); ++i)
    {
      if (ImGui::Selectable(model[i].c_str(), index_select == i))
      {
        index_select = i;
      }
    }

    if (index_select != -1)
    {
      m_context.engine.GetScene().AddModel(model[index_select]);
      index_select = -1;
    }
  }

} // namespace avion::editor::panel