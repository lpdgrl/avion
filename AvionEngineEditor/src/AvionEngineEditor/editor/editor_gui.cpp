#include "AvionEngineEditor/editor/editor_gui.hpp"

namespace avion::editor::gui
{

  EditorGui::EditorGui(EditorContext context)
  : m_editor_context(context)
  , m_inspector_panel(context)
  , m_profiler_panel(ProfileContex{.profiler = context.engine.GetProfiler()})
  , m_hierarchy_panel(context)
  , m_asset_panel(context)
  {

  }


  void EditorGui::Render()
  {
    m_inspector_panel.Render();
    m_profiler_panel.Render();
    m_hierarchy_panel.Render();
    m_asset_panel.Render();
  }

} // namespace avion::editor::gui