#ifndef AVION_EDITOR_HIERARCHY_PANEL_H
#define AVION_EDITOR_HIERARCHY_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"

  namespace avion::editor::panel
  {
    
    class HierarchyPanel
    {
      using ScopeWindow   = ui::avguicontext::AvGuiScopedWindow;
      using EditorContext = detail::EditorContext;

      public:
        HierarchyPanel(EditorContext context);

        void Render();
        
        ~HierarchyPanel() = default;
      private:
        EditorContext m_context;
    };

  } // namespace avion::editor::panel


#endif