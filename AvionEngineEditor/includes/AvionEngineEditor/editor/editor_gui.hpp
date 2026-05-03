#ifndef AVION_EDITOR_EDITOR_GUI_H
#define AVION_EDITOR_EDITOR_GUI_H

#include "AvionEngineEditor/panels/inspector_panel.hpp"
#include "AvionEngineEditor/panels/profiler_panel.hpp"
#include "AvionEngineEditor/panels/hierarchy_panel.hpp"
#include "AvionEngineEditor/panels/asset_panel.hpp"
#include "AvionEngineEditor/panels/scene_panel.hpp"

namespace avion::editor::gui
{
  class EditorGui
  {
    public:
      using EditorContext   = detail::EditorContext;
      using ProfileContex   = detail::ProfilerContext;
      using InspectorPanel  = panel::InspectorPanel;
      using ProfilerPanel   = panel::ProfilerPanel;
      using HierarchyPanel  = panel::HierarchyPanel;
      using AssetPanel      = panel::AssetPanel;
      using ScenePanel      = panel::ScenePanel;

      EditorGui() = delete;
      EditorGui(EditorContext context);

      void Render();

      EditorContext& GetContext() noexcept;
     
      ~EditorGui() = default;

    private:
      EditorContext   m_editor_context;

      InspectorPanel  m_inspector_panel;
      ProfilerPanel   m_profiler_panel;
      HierarchyPanel  m_hierarchy_panel;
      AssetPanel      m_asset_panel;
      ScenePanel      m_scene_panel;

      int m_width_window;
      int m_height_window;
  };

} // namespace avion::editor::app


#endif